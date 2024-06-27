/*This file Handles all the tasks related to AWS  */
/*1)Getting Permanent Keys  */
/*2)Saving Permanent Keys in SPIFF  */
/*3)Registering the thing  */
/*4)Restart and connecting with permanent certificate and sub and pub to topics*/


#include "aws_handling.h"
#include "Webserver.h"

extern char name_aws[50];
extern char loc_aws[50];


WiFiClientSecure net;
PubSubClient client(net);
uint8_t aws_status_flag = 0;


/*Function declarations*/
static void publishMessage();
static void createCertificate();
static void connectToAWS(DynamicJsonDocument cert);
static void messageHandler(String topic, byte *payload, int length);
static void registerThing(DynamicJsonDocument doc);
static void saveCertificateToFS(DynamicJsonDocument doc);


/*This function save permanent certificates */
/*Which we receive from AWS and save it to SPIFF*/
static void saveCertificateToFS(DynamicJsonDocument doc)
{
  DynamicJsonDocument pem(4000);
  pem["certificatePem"] = doc["certificatePem"];
  pem["privateKey"] = doc["privateKey"];
  File file = SPIFFS.open(AWS_SAVE_CERT_SPIFF_LOC, "w");
  if (!file)
  {
    Serial.println("failed to open config file for writing");
  }
  serializeJson(pem, Serial);
  serializeJson(pem, file);
  file.close();
}

/*This function is responsible for creating a thing on aws server*/
/*When this function run properly the registered thing will show up on AWS*/
static void registerThing(DynamicJsonDocument doc)
{

   const char *certificateOwnershipToken = doc["certificateOwnershipToken"];
     Serial.print("certificateOwnershipToken is following    :     ");
  Serial.println(certificateOwnershipToken);
  DynamicJsonDocument reqBody(4000);
  reqBody["certificateOwnershipToken"] = certificateOwnershipToken;
  reqBody["parameters"]["SerialNumber"] = WiFi.macAddress();
  reqBody["parameters"]["Location"] = loc_aws;
  reqBody["parameters"]["Organization"] = name_aws;
  char jsonBuffer[4000];
  serializeJson(reqBody, jsonBuffer);
  Serial.println("Sending certificate..");
  client.publish(AWS_JSON_REGISTER_THING, jsonBuffer);
}

/*This is a callback from data received from MQTT*/
static void messageHandler(String topic, byte *payload, int length)
{
  Serial.print("incoming: ");
  Serial.println(topic);
   //  Serial.println((char *)payload);
  DynamicJsonDocument doc(length);
  deserializeJson(doc, payload);
  if (topic == AWS_JSON_ACCEPTED)
  {
    saveCertificateToFS(doc);
    registerThing(doc);
  }
  else if (topic == AWS_JSON_TEMPLATE_ACCEPTED)
  {
   const char *msg1 = doc["deviceConfiguration"];
       Serial.println("deviceConfiguration is :");
        Serial.println(msg1);
    const char *msg2 = doc["thingName"];
           Serial.println("Thingname is :");
        Serial.println(msg2);
    Serial.println("Register things successfully.");
    Serial.println("Restart in 5s.");
    sleep(5);
    ESP.restart();
  }
  else
  {
 
      const char* message = doc["message"];
      Serial.println(message);
  }
}

static void connectToAWS(DynamicJsonDocument cert)
{
  net.setCACert(AWS_CERT_CA);
  net.setCertificate(cert["certificatePem"]);
  net.setPrivateKey(cert["privateKey"]);
  client.setServer(AWS_IOT_ENDPOINT, 8883);
  // Create a message handler
  client.setCallback(messageHandler);
  client.setBufferSize(4000);
  Serial.print("Connecting to AWS IOT.");
  
   String thingname = String(THING_GROUP) + "_" + WiFi.macAddress();
  char topic1[50];
  thingname.toCharArray(topic1, 50);
  client.connect(THING_GROUP);
  if (!client.connected())
  {
    Serial.println("Timeout!");
    ESP.restart();
  }
  Serial.println("Connected");
  String subscriptionTopic = String(AWS_IOT_SUB_TOPIC) + "_" + WiFi.macAddress();
  char topic[50];
  subscriptionTopic.toCharArray(topic, 50);
  Serial.printf("Subscription topic: %s", topic);
  Serial.println();
  client.subscribe(topic);
}

static void createCertificate()
{
    File download;
    download = SPIFFS.open("/"+cert_pem, "r");
  while (download.available()) {
      //Serial.write(download.read());
      cert_pem_data += char(download.read());
      
    }

   download.close();

    download = SPIFFS.open("/"+private_key, "r");
  while (download.available()) {
      //Serial.write(download.read());
      private_key_data += char(download.read());
      
    }

   download.close();


  Serial.println("No file content.");
  net.setCACert(AWS_CERT_CA);
  net.setCertificate(cert_pem_data.c_str());
  net.setPrivateKey(private_key_data.c_str());
  client.setServer(AWS_IOT_ENDPOINT, 8883);
  // Create a message handler
  client.setCallback(messageHandler);
  // Set buffer size for receive a certificate.
  client.setBufferSize(4000);
  Serial.print("Connecting to AWS IOT.");
  client.connect(THING_GROUP);
  if (!client.connected())
  {
    Serial.println("Timeout!");
    ESP.restart();
  }
  Serial.println("Connected");

  client.subscribe(AWS_JSON_ACCEPTED);
  client.subscribe(AWS_JSON_REJECTED);
  client.subscribe(AWS_JSON_TEMPLATE_ACCEPTED);
  client.subscribe(AWS_JSON_TEMPLATE_REJECTED);
  Serial.println("Create certificate..");
  client.publish(AWS_JSON_, "");
}


static void publishMessage()
{

  StaticJsonDocument<200> doc;
  doc["From ESP32"] = "Working, Connected to ESP32";
  char jsonBuffer[512];
  serializeJson(doc, jsonBuffer); // print to client
 
 // client.publish(topic, jsonBuffer);
  client.publish(AWS_IOT_PUB_TOPIC,jsonBuffer);
}










/*This loop is responsible of all handling all the*/
/*logic related to aws which we want to execute when it connects to aws*/
void awshandling_loop(void)
{
  switch(aws_status_flag)
  {
    case AWS_SERVER_CONNECTED:
    {
     publishMessage();
     client.loop();
     delay(4000);  
      break;
    }

    case AWS_SERVER_NOT_CONNECTED:
    {
      client.loop();
      break;
    }
  }
}

void awshandling_setup()//const char *ssid,const char *pass)
{

  // Read AWS config file.
  File file = SPIFFS.open("/aws.json", "r");

  if (!file)
  {
    Serial.println("Failed to open file for reading");
    return;
  }
  DynamicJsonDocument cert(4000);
  auto deserializeError = deserializeJson(cert, file);
  if (!deserializeError)
  {
    if (cert["certificatePem"])
    {
      connectToAWS(cert);
      aws_status_flag = AWS_SERVER_CONNECTED;
    }
  }
  else
  {
    createCertificate();
    aws_status_flag = AWS_SERVER_NOT_CONNECTED;
  }
  file.close();  
}