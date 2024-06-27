#include <Arduino.h>
#include <SPIFFS.h>
#include <WiFi.h>
#include <ArduinoJson.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <Preferences.h>
#include "aws_handling.h"
#include "ble_handling.h"
#include "Webserver.h"


uint8_t status = 0;

char ssid[50];
char pass[50];

char name_aws[50];
char loc_aws[50];

SET_LOOP_TASK_STACK_SIZE(16*1024);
void setup()
{
  Serial.begin(115200);

   if(blehandling_check_wifi_credentials(ssid,pass))
     {
      WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);
  Serial.print("Connecting to WiFi..");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("Connected.");
  
  Serial.println("\n\nNetwork Configuration:");
  Serial.println("----------------------");
  Serial.print("         SSID: "); Serial.println(WiFi.SSID());
  Serial.print("  Wifi Status: "); Serial.println(WiFi.status());
  Serial.print("Wifi Strength: "); Serial.print(WiFi.RSSI()); Serial.println(" dBm");
  Serial.print("          MAC: "); Serial.println(WiFi.macAddress());
  Serial.print("           IP: "); Serial.println(WiFi.localIP());
  Serial.print("       Subnet: "); Serial.println(WiFi.subnetMask());
  Serial.print("      Gateway: "); Serial.println(WiFi.gatewayIP());
  Serial.print("        DNS 1: "); Serial.println(WiFi.dnsIP(0));
  Serial.print("        DNS 2: "); Serial.println(WiFi.dnsIP(1));
  Serial.print("        DNS 3: "); Serial.println(WiFi.dnsIP(2));
  // Init SPIFFS.
  if (!SPIFFS.begin(true))
  {
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }

  bool exists1 = SPIFFS.exists("/" + cert_pem);
  bool exists2 = SPIFFS.exists("/" + private_key);

  if(exists1 && exists2)
  {

    Serial.println("Following file exists");
  awshandling_setup();
  status = 1;
  }
  else
  {
  
     Serial.println("Following files does not exists");
        webserver_setup();
  }

      }
      else
      {

       blehandling_setup();
           
        status = 2;
     }


      
}

void loop()
{

    switch(status)
  {
    case 1:
    {

     awshandling_loop();
      break;
    }

    case 2:
    {
      blehandling_loop();
      break;
    }

    default:
      {
             webserver_loop();
        break;
      }
  }




}