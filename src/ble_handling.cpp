/*This file Handles all the tasks related to BLE  */
/*1)First check if ssid and password are stored inside NVS flash */
/*2)If not then open ble gatt server  */
/*3)U wait for ssid and password from ble  */
/*4)After getting those restart the esp32 and connect to aws*/

#include "ble_handling.h"

#include <NimBLEDevice.h>

extern char name_aws[50];
extern char loc_aws[50];

// See the following for generating UUIDs:
// https://www.uuidgenerator.net/
#define BLE_RUN 0
#define WIFI_RUN 1
#define SERVICE_UUID "cfc9746f-8c03-4af0-90f9-4697fe03c81a"
#define SERVICE_UUID_2 "9d2eb4a0-a11c-4239-9871-7fbe34536bae"
#define CHARACTERISTIC_UUID_SSID "7687a18b-4358-48cf-91d1-7b5fca5bacae"
#define CHARACTERISTIC_UUID_PASS "b6174e00-5a0c-4326-b0f7-ec0663cd1305"
#define CHARACTERISTIC_UUID_NAME "03ac95eb-38bf-4de4-ae32-47ced60cf2d7"
#define CHARACTERISTIC_UUID_LOCATION "bc7cb6a0-95c3-4a2e-8753-a52da038958c"
#define CHARACTERISTIC_UUID_FLASH "48b79ea3-5cd0-4ab7-9ac5-bbc7c9d57713"

Preferences preferences;


/*GLOBAL Var*/
//BLEServer *pServer = NULL;
static NimBLEServer* pServer;
NimBLECharacteristic *pSSIDcharac;
NimBLECharacteristic *pPASScharac;
NimBLECharacteristic *pNAMEcharac;
NimBLECharacteristic *pLOCATIONcharac;
NimBLECharacteristic *pFLASHcharac;
NimBLECharacteristic *pPRIVATEKEYcharac;
bool deviceConnected = false;
bool oldDeviceConnected = false;
uint16_t handle_ssid;
uint16_t handle_pass;
uint16_t handle_name;
uint16_t handle_location;
uint16_t handle_privatekey;

uint8_t flag_saved = 0;
uint8_t flag_SSID = 0;
uint8_t flag_PASS = 0;
uint8_t flag_NAME = 0;
uint8_t flag_LOCATION = 0;
char SSID[40];
char PASS[40];
char NAME[40];
char LOCATION[40];
String SSID_S;
String PASS_S;
String NAME_S;
String LOCATION_S;



/*CALLBACKS*/

class ServerCallbacks : public NimBLEServerCallbacks {
  void onConnect(NimBLEServer *pServer) {
    deviceConnected = true;
  };

  void onDisconnect(NimBLEServer *pServer) {
    deviceConnected = false;
  }
};

class MyCallbacksSSID : public NimBLECharacteristicCallbacks {
  void onWrite(NimBLECharacteristic *pCharacteristic) {


    std::string rxValue = pCharacteristic->getValue();
 //   Serial.println("SSID CALLBACK");
    if (rxValue.length() > 0) {
     Serial.println("*********");
     Serial.print("Received Value: ");
      for (int i = 0; i < rxValue.length(); i++) {
        Serial.print(rxValue[i]);
      }


      strcpy(SSID, rxValue.c_str());
     Serial.println();
     Serial.println("*********");
    }
    flag_SSID = 1;
  }
    void onRead(BLECharacteristic* pCharacteristic)
  {
    std::string str(SSID);
     pCharacteristic->setValue(str);
  }

};

class MyCallbacksPASS : public NimBLECharacteristicCallbacks {
  void onWrite(NimBLECharacteristic *pCharacteristic) {

    std::string rxValue = pCharacteristic->getValue();
   // Serial.println("PASS CALLBACK");
    if (rxValue.length() > 0) {
     Serial.println("*********");
     Serial.print("Received Value: ");
      for (int i = 0; i < rxValue.length(); i++) {
        Serial.print(rxValue[i]);
      }
      strcpy(PASS, rxValue.c_str());
     Serial.println();
     Serial.println("*********");
    }
    flag_PASS = 1;
  }
    void onRead(BLECharacteristic* pCharacteristic)
  {
    std::string str(PASS);
     pCharacteristic->setValue(str);
  }

};

class MyCallbacksNAME : public NimBLECharacteristicCallbacks {
  void onWrite(NimBLECharacteristic *pCharacteristic) {

    std::string rxValue = pCharacteristic->getValue();
    Serial.println("NAME CALLBACK");
    if (rxValue.length() > 0) {
     Serial.println("*********");
     Serial.print("Received Value: ");
      for (int i = 0; i < rxValue.length(); i++) {
        Serial.print(rxValue[i]);
      }
      strcpy(NAME, rxValue.c_str());
     Serial.println();
     Serial.println("*********");
    }
    flag_NAME = 1;
  }

      void onRead(BLECharacteristic* pCharacteristic)
  {
    std::string str(NAME);
     pCharacteristic->setValue(str);
  }
};

class MyCallbacksLOCATION: public NimBLECharacteristicCallbacks {
  void onWrite(NimBLECharacteristic *pCharacteristic) {

    std::string rxValue = pCharacteristic->getValue();
    Serial.println("Location CALLBACK");
    if (rxValue.length() > 0) {
      Serial.println("*********");
      Serial.print("Received Value: ");
      for (int i = 0; i < rxValue.length(); i++) {
        Serial.print(rxValue[i]);
      }
      strcpy(LOCATION, rxValue.c_str());
     Serial.println();
     Serial.println("*********");
    }
    flag_LOCATION = 1;
  }
      void onRead(BLECharacteristic* pCharacteristic)
  {
    std::string str(LOCATION);
     pCharacteristic->setValue(str);
  }


   

};

char privatekey[1400];
class MyCallbacksPRIVATEKEY: public NimBLECharacteristicCallbacks {
  void onWrite(NimBLECharacteristic *pCharacteristic) {

    std::string rxValue = pCharacteristic->getValue();
    Serial.println("PRIVATEKEY CALLBACK");
    if (rxValue.length() > 0) {
      Serial.println("*********");
      Serial.print("Received Value: ");
      for (int i = 0; i < rxValue.length(); i++) {
        Serial.print(rxValue[i]);
      }
      strcpy(privatekey, rxValue.c_str());
          //  Serial.print(privatekey);
     Serial.println();
     Serial.println("*********");
    }
   // flag_LOCATION = 1;
  }
      void onRead(BLECharacteristic* pCharacteristic)
  {
    std::string str(LOCATION);
     pCharacteristic->setValue(str);
  }


   

};











 void blehandling_loop(void)
{
 
  if (!deviceConnected && oldDeviceConnected) {
    delay(500);                   // give the bluetooth stack the chance to get things ready
    pServer->startAdvertising();  // restart advertising
    Serial.println("start advertising");
    oldDeviceConnected = deviceConnected;
  }
  // connecting
  if (deviceConnected && !oldDeviceConnected) {
    // do stuff here on connecting
    oldDeviceConnected = deviceConnected;
  }

  if (deviceConnected) {

    if (flag_PASS) {
      Serial.println("inside flag passs");
      Serial.println(PASS);
     preferences.putString("password", PASS);
    

      flag_PASS = 0;
      flag_saved++;
    }
    if (flag_SSID) {
      Serial.println("inside flag ssid");
      Serial.println(SSID);
     preferences.putString("ssid", SSID);
     
      flag_SSID = 0;
      flag_saved++;
    }

      if (flag_NAME) {
     Serial.println("inside flag NAME");
     Serial.println(NAME);
     preferences.putString("name", NAME);

      flag_NAME = 0;
      flag_saved++;
    }
    if (flag_LOCATION) {
      Serial.println("inside flag LOCATION");
      Serial.println(LOCATION);
      preferences.putString("location", LOCATION);
      flag_LOCATION = 0;
      flag_saved++;
    }

    if (flag_saved >= 4) {
      Serial.println("inside flag saved");
      flag_saved = 0;
      uint8_t data[] = "credentials saved";
      delay(5000);
      ESP.restart();
    }
  }

   delay(1000);  

}
 void blehandling_setup(void)
{

   uint64_t chipid = ESP.getEfuseMac(); // The chip ID is essentially its MAC address(length: 6 bytes).
  uint16_t chip = (uint16_t)(chipid >> 32);
   char name[50];

  sprintf(name,"ESP32_AWS_%d",chip);

   std::string name_ble(name);
  NimBLEDevice::init(name_ble);

  pServer = NimBLEDevice::createServer();
  pServer->setCallbacks(new ServerCallbacks());
 
  NimBLEService* pService = pServer->createService("DEAD");
  
  pSSIDcharac =  pService->createCharacteristic(
                                               "BEEF",
                                               NIMBLE_PROPERTY::READ |
                                               NIMBLE_PROPERTY::WRITE 
                                              );

  handle_ssid = pSSIDcharac->getHandle();
  pSSIDcharac->setCallbacks(new MyCallbacksSSID());

    pPASScharac =  pService->createCharacteristic(
                                               "BEEF",
                                               NIMBLE_PROPERTY::READ |
                                               NIMBLE_PROPERTY::WRITE 
                                              );

  handle_pass = pPASScharac->getHandle();
  pPASScharac->setCallbacks(new MyCallbacksPASS());


  pNAMEcharac =  pService->createCharacteristic(
                                               "BEEF",
                                               NIMBLE_PROPERTY::READ |
                                               NIMBLE_PROPERTY::WRITE 
                                              );

  handle_name = pNAMEcharac->getHandle();
  pNAMEcharac->setCallbacks(new MyCallbacksNAME());


  pLOCATIONcharac =  pService->createCharacteristic(
                                               "BEEF",
                                               NIMBLE_PROPERTY::READ |
                                               NIMBLE_PROPERTY::WRITE 
                                              );

  handle_location = pLOCATIONcharac->getHandle();
  pLOCATIONcharac->setCallbacks(new MyCallbacksLOCATION());




  pService->start();
  pServer->getAdvertising()->start();
}


uint8_t blehandling_check_wifi_credentials(char *ssid,char *pass)
{
      preferences.begin("my-app", false);


  SSID_S = preferences.getString("ssid", "");
  PASS_S = preferences.getString("password", "");
  NAME_S = preferences.getString("name", "");
  LOCATION_S = preferences.getString("location", "");

  Serial.println(SSID_S);
    Serial.println(PASS_S);
      Serial.println(NAME_S);
        Serial.println(LOCATION_S);

    if (SSID_S == "" || PASS_S == "") {

        return 0;
    }
    else
    {
         strcpy(ssid, SSID_S.c_str());
         strcpy(pass, PASS_S.c_str());

         strcpy(name_aws, NAME_S.c_str());
         strcpy(loc_aws, LOCATION_S.c_str());
        return 1;
    }
}







































