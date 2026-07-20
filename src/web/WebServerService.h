#pragma once
#include <WebServer.h>
#include "display/DisplayManager.h"
#include "network/WiFiService.h"
#include "timing/StopwatchService.h"
#include "timing/CountdownService.h"
class WebServerService { public: WebServerService(DisplayManager& d,WiFiService& w,StopwatchService& s,CountdownService& t):display_(d),wifi_(w),stopwatch_(s),timer_(t){} void begin();void loop(); private: void status();void command();void json(bool ok,const String& data,const String& error="null",int code=200); WebServer server_{80};DisplayManager& display_;WiFiService& wifi_;StopwatchService& stopwatch_;CountdownService& timer_; };
