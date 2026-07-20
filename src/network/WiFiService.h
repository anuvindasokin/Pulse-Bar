#pragma once
#include <Arduino.h>
#include <DNSServer.h>
#include <Preferences.h>
#include <WiFi.h>
class WiFiService {
 public:
  void begin();
  void loop();
  bool connected()const{return WiFi.status()==WL_CONNECTED;}
  bool setupMode()const{return apMode_;}
  bool hasCredentials()const{return ssid_.length()>0;}
  bool configure(const String& ssid,const String& password,const String& deviceName,const String& timezone);
  String scanJson();
  String ip()const;
  String modeName()const{return apMode_?"setup":"station";}
  String deviceName()const{return deviceName_;}
  String timezone()const{return timezone_;}
  String ssid()const{return connected()?WiFi.SSID():String();}
  int rssi()const{return connected()?WiFi.RSSI():0;}
 private:
  void startSetupPortal();
  void startStation();
  Preferences preferences_;
  DNSServer dns_;
  String ssid_,password_,deviceName_="PulseBar",timezone_="UTC0";
  bool apMode_=false;
  uint32_t lastAttempt_=0;
};
