#include "network/WiFiService.h"
#include "BuildConfig.h"
#include <ESPmDNS.h>
#include <ArduinoJson.h>

void WiFiService::begin(){
 preferences_.begin("pulsebar",false);
 ssid_=preferences_.getString("ssid","");password_=preferences_.getString("wifiPass","");
 deviceName_=preferences_.getString("deviceName","PulseBar");timezone_=preferences_.getString("timezone","UTC0");
 setenv("TZ",timezone_.c_str(),1);tzset();
 if(ssid_.isEmpty())startSetupPortal();else startStation();
}
void WiFiService::startStation(){WiFi.mode(WIFI_STA);WiFi.setHostname(PULSEBAR_DEFAULT_HOSTNAME);WiFi.begin(ssid_.c_str(),password_.c_str());lastAttempt_=millis();apMode_=false;}
void WiFiService::startSetupPortal(){
 WiFi.mode(WIFI_AP_STA);WiFi.softAP(PULSEBAR_SETUP_SSID);dns_.start(53,"*",WiFi.softAPIP());apMode_=true;
 Serial.printf("[wifi] Connect to %s and open http://192.168.4.1\n",PULSEBAR_SETUP_SSID);
}
void WiFiService::loop(){
 if(apMode_)dns_.processNextRequest();
 if(connected()){if(apMode_){dns_.stop();WiFi.softAPdisconnect(true);apMode_=false;}static bool mdns=false;if(!mdns){MDNS.begin(PULSEBAR_DEFAULT_HOSTNAME);MDNS.addService("http","tcp",80);mdns=true;}return;}
 if(!apMode_&&millis()-lastAttempt_>20000)startSetupPortal();
}
bool WiFiService::configure(const String& ssid,const String& password,const String& deviceName,const String& timezone){
 if(ssid.isEmpty()||ssid.length()>32||password.length()>63||deviceName.length()>32||timezone.length()>64)return false;
 ssid_=ssid;password_=password;deviceName_=deviceName.isEmpty()?"PulseBar":deviceName;timezone_=timezone.isEmpty()?"UTC0":timezone;
 preferences_.putString("ssid",ssid_);preferences_.putString("wifiPass",password_);preferences_.putString("deviceName",deviceName_);preferences_.putString("timezone",timezone_);
 setenv("TZ",timezone_.c_str(),1);tzset();startStation();return true;
}
String WiFiService::scanJson(){
 JsonDocument root;JsonArray list=root.to<JsonArray>();int count=WiFi.scanNetworks(false,true);
 for(int i=0;i<count&&i<20;i++){JsonObject n=list.add<JsonObject>();n["ssid"]=WiFi.SSID(i);n["rssi"]=WiFi.RSSI(i);n["secure"]=WiFi.encryptionType(i)!=WIFI_AUTH_OPEN;}
 WiFi.scanDelete();String out;serializeJson(root,out);return out;
}
String WiFiService::ip()const{return connected()?WiFi.localIP().toString():WiFi.softAPIP().toString();}
