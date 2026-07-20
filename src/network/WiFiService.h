#pragma once
#include <Arduino.h>
#include <WiFi.h>
class WiFiService { public: void begin(); void loop(); bool connected()const{return WiFi.status()==WL_CONNECTED;} String ip()const; String modeName()const{return apMode_?"setup":"station";} int rssi()const{return connected()?WiFi.RSSI():0;} private: bool apMode_=false; uint32_t lastAttempt_=0; };
