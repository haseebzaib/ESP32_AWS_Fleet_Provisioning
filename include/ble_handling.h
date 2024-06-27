











#include <Arduino.h>
// #include <BLEDevice.h>
// #include <BLEServer.h>
// #include <BLEUtils.h>
 #include <Preferences.h>
// #include <BLE2902.h>
// #include <WiFi.h>

// extern char SSID[40];
// extern char PASS[40];
// extern char NAME[40];
// extern char LOCATION[40];
// extern String NAME_S;
// extern String LOCATION_S;


extern void blehandling_loop(void);
extern void blehandling_setup(void);
extern uint8_t blehandling_check_wifi_credentials( char *ssid,char *pass);







// #include <Arduino.h>
// #include <BLEDevice.h>
// #include <BLEServer.h>
// #include <BLEUtils.h>
// #include <Preferences.h>
// #include <BLE2902.h>
// #include <WiFi.h>

// // extern char SSID[40];
// // extern char PASS[40];
// // extern char NAME[40];
// // extern char LOCATION[40];
// // extern String NAME_S;
// // extern String LOCATION_S;


// extern void blehandling_loop(void);
// extern void blehandling_setup(void);
// extern uint8_t blehandling_check_wifi_credentials( char *ssid,char *pass);