#include "web/WebServerService.h"
#include <ArduinoJson.h>
#include "Version.h"
#include "web/WebUi.h"

void WebServerService::begin(){
 server_.on("/",HTTP_GET,[this](){server_.send_P(200,"text/html",WEB_UI);});
 server_.on("/api/v1/status",HTTP_GET,[this](){status();});
 server_.on("/api/v1/control",HTTP_POST,[this](){command();});
 server_.onNotFound([this](){json(false,"null","{\"code\":\"NOT_FOUND\",\"message\":\"Route not found\"}",404);});
 server_.begin();
}
void WebServerService::loop(){server_.handleClient();}
void WebServerService::json(bool ok,const String& data,const String& error,int code){server_.send(code,"application/json",String("{\"success\":")+(ok?"true":"false")+",\"data\":"+data+",\"error\":"+error+"}");}
void WebServerService::status(){JsonDocument d;d["name"]="PulseBar";d["version"]=PULSEBAR_VERSION;d["uptime"]=millis()/1000;d["wifi"]=wifi_.connected();d["mode"]=wifi_.modeName();d["ip"]=wifi_.ip();d["rssi"]=wifi_.rssi();d["brightness"]=display_.brightness();d["stopwatch"]["running"]=stopwatch_.running();d["stopwatch"]["elapsedMs"]=stopwatch_.elapsedMs();d["timer"]["running"]=timer_.running();d["timer"]["remainingMs"]=timer_.remainingMs();String out;serializeJson(d,out);json(true,out);}
void WebServerService::command(){
 if(!server_.hasArg("plain")){json(false,"null","{\"code\":\"BODY_REQUIRED\",\"message\":\"JSON body required\"}",400);return;}
 JsonDocument d;if(deserializeJson(d,server_.arg("plain"))){json(false,"null","{\"code\":\"INVALID_JSON\",\"message\":\"Invalid JSON\"}",400);return;}
 String a=d["action"]|"";
 if(a=="brightness")display_.setBrightness(constrain(d["value"]|48,1,255));else if(a=="displayTest")display_.testPattern();else if(a=="stopwatchStart")stopwatch_.start();else if(a=="stopwatchPause")stopwatch_.pause();else if(a=="stopwatchReset")stopwatch_.reset();else if(a=="timerStart")timer_.start(constrain(d["seconds"]|60,1,359999));else if(a=="timerPause")timer_.pause();else if(a=="timerResume")timer_.resume();else if(a=="timerReset")timer_.reset();else{json(false,"null","{\"code\":\"UNKNOWN_ACTION\",\"message\":\"Unknown control action\"}",400);return;}json(true,"{}");
}
