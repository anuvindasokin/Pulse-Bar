#pragma once
#include <Preferences.h>
#include <WebServer.h>
#include "display/DisplayManager.h"
#include "display/PlaylistService.h"
#include "network/WiFiService.h"
#include "timing/StopwatchService.h"
#include "timing/CountdownService.h"
class WebServerService { public: WebServerService(DisplayManager& d,WiFiService& w,StopwatchService& s,CountdownService& t,PlaylistService& p):display_(d),wifi_(w),stopwatch_(s),timer_(t),playlist_(p){} void begin();void loop();bool updateSocialMetrics();int64_t socialMetric(uint8_t scene)const{return scene<6?socialMetrics_[scene]:0;} private: void status();void command();void provision();void accounts();void saveAccount();void savePlaylist();void connectStatus();void saveConnectConfig();void connectLink();void connectMetrics();bool connectRequest(const String& path,String& response);void json(bool ok,const String& data,const String& error="null",int code=200); WebServer server_{80};Preferences accountStore_;int64_t socialMetrics_[6]={};DisplayManager& display_;WiFiService& wifi_;StopwatchService& stopwatch_;CountdownService& timer_;PlaylistService& playlist_; };
