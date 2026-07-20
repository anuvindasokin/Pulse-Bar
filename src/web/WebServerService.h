#pragma once
#include <Preferences.h>
#include <WebServer.h>
#include "display/DisplayManager.h"
#include "display/PlaylistService.h"
#include "network/WiFiService.h"
#include "timing/StopwatchService.h"
#include "timing/CountdownService.h"
class WebServerService { public: WebServerService(DisplayManager& d,WiFiService& w,StopwatchService& s,CountdownService& t,PlaylistService& p):display_(d),wifi_(w),stopwatch_(s),timer_(t),playlist_(p){} void begin();void loop(); private: void status();void command();void provision();void accounts();void saveAccount();void savePlaylist();void json(bool ok,const String& data,const String& error="null",int code=200); WebServer server_{80};Preferences accountStore_;DisplayManager& display_;WiFiService& wifi_;StopwatchService& stopwatch_;CountdownService& timer_;PlaylistService& playlist_; };
