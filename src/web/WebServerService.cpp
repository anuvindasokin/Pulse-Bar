#include "web/WebServerService.h"
#include <ArduinoJson.h>
#include "Version.h"
#include "web/WebUi.h"
#include <HTTPClient.h>
#include <WiFiClientSecure.h>

static const char PULSEBAR_CONNECT_CA[] PROGMEM=R"PEM(-----BEGIN CERTIFICATE-----
MIIFazCCA1OgAwIBAgIRAIIQz7DSQONZRGPgu2OCiwAwDQYJKoZIhvcNAQELBQAw
TzELMAkGA1UEBhMCVVMxKTAnBgNVBAoTIEludGVybmV0IFNlY3VyaXR5IFJlc2Vh
cmNoIEdyb3VwMRUwEwYDVQQDEwxJU1JHIFJvb3QgWDEwHhcNMTUwNjA0MTEwNDM4
WhcNMzUwNjA0MTEwNDM4WjBPMQswCQYDVQQGEwJVUzEpMCcGA1UEChMgSW50ZXJu
ZXQgU2VjdXJpdHkgUmVzZWFyY2ggR3JvdXAxFTATBgNVBAMTDElTUkcgUm9vdCBY
MTCCAiIwDQYJKoZIhvcNAQEBBQADggIPADCCAgoCggIBAK3oJHP0FDfzm54rVygc
h77ct984kIxuPOZXoHj3dcKi/vVqbvYATyjb3miGbESTtrFj/RQSa78f0uoxmyF+
0TM8ukj13Xnfs7j/EvEhmkvBioZxaUpmZmyPfjxwv60pIgbz5MDmgK7iS4+3mX6U
A5/TR5d8mUgjU+g4rk8Kb4Mu0UlXjIB0ttov0DiNewNwIRt18jA8+o+u3dpjq+sW
T8KOEUt+zwvo/7V3LvSye0rgTBIlDHCNAymg4VMk7BPZ7hm/ELNKjD+Jo2FR3qyH
B5T0Y3HsLuJvW5iB4YlcNHlsdu87kGJ55tukmi8mxdAQ4Q7e2RCOFvu396j3x+UC
B5iPNgiV5+I3lg02dZ77DnKxHZu8A/lJBdiB3QW0KtZB6awBdpUKD9jf1b0SHzUv
KBds0pjBqAlkd25HN7rOrFleaJ1/ctaJxQZBKT5ZPt0m9STJEadao0xAH0ahmbWn
OlFuhjuefXKnEgV4We0+UXgVCwOPjdAvBbI+e0ocS3MFEvzG6uBQE3xDk3SzynTn
jh8BCNAw1FtxNrQHusEwMFxIt4I7mKZ9YIqioymCzLq9gwQbooMDQaHWBfEbwrbw
qHyGO0aoSCqI3Haadr8faqU9GY/rOPNk3sgrDQoo//fb4hVC1CLQJ13hef4Y53CI
rU7m2Ys6xt0nUW7/vGT1M0NPAgMBAAGjQjBAMA4GA1UdDwEB/wQEAwIBBjAPBgNV
HRMBAf8EBTADAQH/MB0GA1UdDgQWBBR5tFnme7bl5AFzgAiIyBpY9umbbjANBgkq
hkiG9w0BAQsFAAOCAgEAVR9YqbyyqFDQDLHYGmkgJykIrGF1XIpu+ILlaS/V9lZL
ubhzEFnTIZd+50xx+7LSYK05qAvqFyFWhfFQDlnrzuBZ6brJFe+GnY+EgPbk6ZGQ
3BebYhtF8GaV0nxvwuo77x/Py9auJ/GpsMiu/X1+mvoiBOv/2X/qkSsisRcOj/KK
NFtY2PwByVS5uCbMiogziUwthDyC3+6WVwW6LLv3xLfHTjuCvjHIInNzktHCgKQ5
ORAzI4JMPJ+GslWYHb4phowim57iaztXOoJwTdwJx4nLCgdNbOhdjsnvzqvHu7Ur
TkXWStAmzOVyyghqpZXjFaH3pO3JLF+l+/+sKAIuvtd7u+Nxe5AW0wdeRlN8NwdC
jNPElpzVmbUq4JUagEiuTDkHzsxHpFKVK7q4+63SM1N95R1NbdWhscdCb+ZAJzVc
oyi3B43njTOQ5yOf+1CceWxG1bQVs5ZufpsMljq4Ui0/1lvh+wjChP4kqKOJ2qxq
4RgqsahDYVvTH9w7jXbyLeiNdd8XM2w9U/t7y0Ff/9yi0GE44Za4rF2LN9d11TPA
mRGunUHBcnWEvgJBQl9nJEiU0Zsnvgc/ubhPgXRR4Xq37Z0j4r7g1SgEEzwxA57d
emyPxgcYxn/eR44/KJ4EBs+lVDR3veyJm+kXQ99b21/+jh5Xos1AnX5iItreGCc=
-----END CERTIFICATE-----)PEM";

void WebServerService::begin(){
 accountStore_.begin("accounts",false);
 server_.on("/",HTTP_GET,[this](){server_.send_P(200,"text/html",WEB_UI);});
 server_.on("/api/v1/status",HTTP_GET,[this](){status();});
 server_.on("/api/v1/control",HTTP_POST,[this](){command();});
 server_.on("/api/v1/network/scan",HTTP_GET,[this](){json(true,wifi_.scanJson());});
 server_.on("/api/v1/setup",HTTP_POST,[this](){provision();});
 server_.on("/api/v1/accounts",HTTP_GET,[this](){accounts();});
 server_.on("/api/v1/accounts",HTTP_POST,[this](){saveAccount();});
 server_.on("/api/v1/connect/status",HTTP_GET,[this](){connectStatus();});
 server_.on("/api/v1/connect/config",HTTP_POST,[this](){saveConnectConfig();});
 server_.on("/api/v1/connect/link",HTTP_POST,[this](){connectLink();});
 server_.on("/api/v1/metrics",HTTP_GET,[this](){connectMetrics();});
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
 JsonDocument d;bool metaAuthorized=accountStore_.getString("metaSecret","").length()>0;for(const char* platform:{"youtube","facebook","instagram"}){String key=String(platform)+"Url",secretKey=String(platform)+"Secret";d[platform]["url"]=accountStore_.getString(key.c_str(),"");bool linked=d[platform]["url"].as<String>().length()>0,authorized=String(platform)=="youtube"?accountStore_.getString(secretKey.c_str(),"").length()>0:metaAuthorized||accountStore_.getString(secretKey.c_str(),"").length()>0;d[platform]["connected"]=linked;d[platform]["authorized"]=authorized;d[platform]["status"]=linked&&authorized?"Connected":linked?"Link saved — connect Meta once":"Not connected";}d["meta"]["authorized"]=metaAuthorized;
 String out;serializeJson(d,out);json(true,out);
}
void WebServerService::saveAccount(){
 JsonDocument d;if(!server_.hasArg("plain")||deserializeJson(d,server_.arg("plain"))){json(false,"null","{\"code\":\"INVALID_JSON\",\"message\":\"Valid account details are required\"}",400);return;}
 String platform=d["platform"]|"",url=d["url"]|"",credential=d["credential"]|"";
 if(platform!="youtube"&&platform!="facebook"&&platform!="instagram"){json(false,"null","{\"code\":\"INVALID_PROVIDER\",\"message\":\"Choose YouTube, Facebook, or Instagram\"}",400);return;}
 if(url.length()>256||(!url.startsWith("https://")&&!url.startsWith("http://"))){json(false,"null","{\"code\":\"INVALID_URL\",\"message\":\"Enter a complete channel link\"}",400);return;}
 String key=platform+"Url",secretKey=platform+"Secret";accountStore_.putString(key.c_str(),url);if(credential.length()>0&&credential.length()<=512){if(platform=="facebook"||platform=="instagram")accountStore_.putString("metaSecret",credential);else accountStore_.putString(secretKey.c_str(),credential);}json(true,"{\"message\":\"Account settings saved\"}");
}
void WebServerService::connectStatus(){JsonDocument d;d["configured"]=accountStore_.getString("connectUrl","").length()>0&&accountStore_.getString("deviceToken","").length()>0;d["backendUrl"]=accountStore_.getString("connectUrl","");d["deviceId"]=accountStore_.getString("deviceId","");String out;serializeJson(d,out);json(true,out);}
void WebServerService::saveConnectConfig(){JsonDocument d;if(!server_.hasArg("plain")||deserializeJson(d,server_.arg("plain"))){json(false,"null","{\"code\":\"INVALID_JSON\",\"message\":\"Valid PulseBar Connect settings are required\"}",400);return;}String url=d["backendUrl"]|"",deviceId=d["deviceId"]|"",token=d["deviceToken"]|"";while(url.endsWith("/"))url.remove(url.length()-1);if(!url.startsWith("https://")||url.length()>180||deviceId.length()<3||deviceId.length()>64||token.length()<16||token.length()>160){json(false,"null","{\"code\":\"INVALID_CONNECT_CONFIG\",\"message\":\"Use an HTTPS backend URL and valid device credentials\"}",400);return;}accountStore_.putString("connectUrl",url);accountStore_.putString("deviceId",deviceId);accountStore_.putString("deviceToken",token);json(true,"{\"configured\":true}");}
bool WebServerService::connectRequest(const String& path,String& response){if(!wifi_.connected())return false;String base=accountStore_.getString("connectUrl",""),deviceId=accountStore_.getString("deviceId",""),token=accountStore_.getString("deviceToken","");if(base.isEmpty()||deviceId.isEmpty()||token.isEmpty())return false;WiFiClientSecure client;client.setCACert(PULSEBAR_CONNECT_CA);HTTPClient http;if(!http.begin(client,base+path+"?deviceId="+deviceId))return false;http.addHeader("Authorization","Bearer "+token);int code=http.GET();response=http.getString();http.end();return code>=200&&code<300;}
void WebServerService::connectLink(){String response;if(!connectRequest("/v1/connect/meta",response)){json(false,"null","{\"code\":\"CONNECT_FAILED\",\"message\":\"Configure PulseBar Connect and check the internet connection\"}",502);return;}json(true,response);}
bool WebServerService::updateSocialMetrics(){String response;if(!connectRequest("/v1/metrics",response))return false;JsonDocument d;if(deserializeJson(d,response))return false;socialMetrics_[2]=d["facebook"]["followers"]|0;socialMetrics_[3]=d["facebook"]["views"]|0;socialMetrics_[4]=d["instagram"]["followers"]|0;socialMetrics_[5]=d["instagram"]["views"]|0;return true;}
void WebServerService::connectMetrics(){String response;if(!connectRequest("/v1/metrics",response)){json(false,"null","{\"code\":\"METRICS_FAILED\",\"message\":\"Metrics are unavailable or the Meta account is not connected\"}",502);return;}JsonDocument d;if(!deserializeJson(d,response)){socialMetrics_[2]=d["facebook"]["followers"]|0;socialMetrics_[3]=d["facebook"]["views"]|0;socialMetrics_[4]=d["instagram"]["followers"]|0;socialMetrics_[5]=d["instagram"]["views"]|0;}json(true,response);}
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
