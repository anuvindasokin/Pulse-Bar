#include "network/WiFiService.h"
#include "BuildConfig.h"
void WiFiService::begin(){WiFi.mode(WIFI_STA);WiFi.setHostname(PULSEBAR_DEFAULT_HOSTNAME);WiFi.begin();lastAttempt_=millis();}
void WiFiService::loop(){if(connected())return;if(!apMode_&&millis()-lastAttempt_>12000){WiFi.mode(WIFI_AP_STA);WiFi.softAP(PULSEBAR_SETUP_SSID);apMode_=true;Serial.printf("[wifi] setup AP at %s\n",WiFi.softAPIP().toString().c_str());}}
String WiFiService::ip()const{return connected()?WiFi.localIP().toString():WiFi.softAPIP().toString();}
