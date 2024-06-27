#include <Arduino.h>


extern String cert_pem;
extern String private_key; 
extern String cert_pem_data;
extern String private_key_data; 


void webserver_loop();
void webserver_setup();