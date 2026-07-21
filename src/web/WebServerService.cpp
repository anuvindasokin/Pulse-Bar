#include "web/WebServerService.h"
#include <ArduinoJson.h>
#include "Version.h"
#include "web/WebUi.h"

void WebServerService::begin(){
 accountStore_.begin("accounts",false);
 server_.on("/",HTTP_GET,[this](){server_.send_P(200,"text/html",WEB_UI);});
 server_.on("/api/v1/status",HTTP_GET,[this](){status();});
 server_.on("/api/v1/control",HTTP_POST,[this](){command();});
 server_.on("/api/v1/network/scan",HTTP_GET,[this](){json(true,wifi_.scanJson());});
 server_.on("/api/v1/setup",HTTP_POST,[this](){provision();});
 server_.on("/api/v1/accounts",HTTP_GET,[this](){accounts();});
 server_.on("/api/v1/accounts",HTTP_POST,[this](){saveAccount();});
 server_.on("/api/v1/playlist",HTTP_GET,[this](){json(true,playlist_.toJson());});
 server_.on("/api/v1/playlist",HTTP_PUT,[this](){savePlaylist();});
 server_.on("/generate_204",HTTP_GET,[this](){server_.sendHeader("Location","http://192.168.4.1/",true);server_.send(302,"text/plain","");});
 server_.on("/hotspot-detect.html",HTTP_GET,[this](){server_.send_P(200,"text/html",WEB_UI);});
 server_.on("/connecttest.txt",HTTP_GET,[this](){server_.sendHeader("Location","http://192.168.4.1/",true);server_.send(302,"text/plain","");});
 server_.onNotFound([this](){if(wifi_.setupMode()){server_.sendHeader("Location","http://192.168.4.1/",true);server_.send(302,"text/plain","");}else json(false,"null","{\"code\":\"NOT_FOUND\",\"message\":\"Route not found\"}",404);});
 server_.begin();
}
void WebServerService::savePlaylist(){if(!server_.hasArg("plain")||!playlist_.updateFromJson(server_.arg("plain"))){json(false,"null","{\"code\":\"INVALID_PLAYLIST\",\"message\":\"Select valid loop scenes\"}",400);return;}json(true,playlist_.toJson());}
void WebServerService::accounts(){
 JsonDocument d;for(const char* platform:{"youtube","facebook","instagram"}){String key=String(platform)+"Url";d[platform]["url"]=accountStore_.getString(key.c_str(),"");d[platform]["connected"]=d[platform]["url"].as<String>().length()>0;d[platform]["status"]=d[platform]["connected"].as<bool>()?"Link saved — API authorization required for live metrics":"Not connected";}
 String out;serializeJson(d,out);json(true,out);
}
void WebServerService::saveAccount(){
 JsonDocument d;if(!server_.hasArg("plain")||deserializeJson(d,server_.arg("plain"))){json(false,"null","{\"code\":\"INVALID_JSON\",\"message\":\"Valid account details are required\"}",400);return;}
 String platform=d["platform"]|"",url=d["url"]|"";
 if(platform!="youtube"&&platform!="facebook"&&platform!="instagram"){json(false,"null","{\"code\":\"INVALID_PROVIDER\",\"message\":\"Choose YouTube, Facebook, or Instagram\"}",400);return;}
 if(url.length()>256||(!url.startsWith("https://")&&!url.startsWith("http://"))){json(false,"null","{\"code\":\"INVALID_URL\",\"message\":\"Enter a complete channel link\"}",400);return;}
 String key=platform+"Url";accountStore_.putString(key.c_str(),url);json(true,"{\"message\":\"Channel link saved\"}");
}
void WebServerService::loop(){server_.handleClient();}
void WebServerService::json(bool ok,const String& data,const String& error,int code){server_.send(code,"application/json",String("{\"success\":")+(ok?"true":"false")+",\"data\":"+data+",\"error\":"+error+"}");}
void WebServerService::status(){JsonDocument d;d["name"]=wifi_.deviceName();d["version"]=PULSEBAR_VERSION;d["uptime"]=millis()/1000;d["wifi"]=wifi_.connected();d["setupRequired"]=!wifi_.hasCredentials();d["mode"]=wifi_.modeName();d["ssid"]=wifi_.ssid();d["ip"]=wifi_.ip();d["rssi"]=wifi_.rssi();d["timezone"]=wifi_.timezone();d["brightness"]=display_.brightness();char color[8];snprintf(color,sizeof(color),"#%06lX",(unsigned long)display_.color());d["color"]=color;d["stopwatch"]["running"]=stopwatch_.running();d["stopwatch"]["elapsedMs"]=stopwatch_.elapsedMs();d["timer"]["running"]=timer_.running();d["timer"]["remainingMs"]=timer_.remainingMs();String out;serializeJson(d,out);json(true,out);}
void WebServerService::provision(){
 JsonDocument d;if(!server_.hasArg("plain")||deserializeJson(d,server_.arg("plain"))){json(false,"null","{\"code\":\"INVALID_JSON\",\"message\":\"Valid setup details are required\"}",400);return;}
 String ssid=d["ssid"]|"",password=d["password"]|"",name=d["deviceName"]|"PulseBar",timezone=d["timezone"]|"UTC0";
 if(!wifi_.configure(ssid,password,name,timezone)){json(false,"null","{\"code\":\"INVALID_SETUP\",\"message\":\"Check the network and device fields\"}",400);return;}json(true,"{\"message\":\"PulseBar is connecting. You can close this page.\"}");
}
void WebServerService::command(){
 if(!server_.hasArg("plain")){json(false,"null","{\"code\":\"BODY_REQUIRED\",\"message\":\"JSON body required\"}",400);return;}
 JsonDocument d;if(deserializeJson(d,server_.arg("plain"))){json(false,"null","{\"code\":\"INVALID_JSON\",\"message\":\"Invalid JSON\"}",400);return;}
 String a=d["action"]|"";
 if(a=="brightness")display_.setBrightness(constrain(d["value"]|48,1,255));else if(a=="color"){String value=d["value"]|"#FF4D91";display_.setColor(strtoul(value.substring(value.startsWith("#")?1:0).c_str(),nullptr,16));}else if(a=="displayTest")display_.testPattern();else if(a=="stopwatchStart")stopwatch_.start();else if(a=="stopwatchPause")stopwatch_.pause();else if(a=="stopwatchReset")stopwatch_.reset();else if(a=="timerStart")timer_.start(constrain(d["seconds"]|60,1,359999));else if(a=="timerPause")timer_.pause();else if(a=="timerResume")timer_.resume();else if(a=="timerReset")timer_.reset();else{json(false,"null","{\"code\":\"UNKNOWN_ACTION\",\"message\":\"Unknown control action\"}",400);return;}json(true,"{}");
}
