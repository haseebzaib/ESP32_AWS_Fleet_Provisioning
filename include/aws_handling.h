/*This file Handles all the tasks related to AWS  */
/*1)Getting Permanent Keys  */
/*2)Saving Permanent Keys in SPIFF  */
/*3)Registering the thing  */
/*4)Restart and connecting with permanent certificate and sub and pub to topics*/



#include <Arduino.h>
#include <SPIFFS.h>
#include <WiFi.h>
#include <ArduinoJson.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include "config.h"


enum aws_status {
    AWS_SERVER_CONNECTED = 1,
    AWS_SERVER_NOT_CONNECTED,
};


extern void awshandling_loop(void);
extern void awshandling_setup();//const char *ssid,const char *pass);






