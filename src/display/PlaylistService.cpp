#include "display/PlaylistService.h"
#include <ArduinoJson.h>

static const char* IDS[PlaylistService::SCENE_COUNT]={"youtubeSubscribers","youtubeViews","facebookFollowers","facebookViews","instagramFollowers","instagramViews","time"};
void PlaylistService::begin(){store_.begin("playlist",false);uint8_t mask=store_.getUChar("mask",0x7F);for(uint8_t i=0;i<SCENE_COUNT;i++)enabled_[i]=mask&(1<<i);}
uint8_t PlaylistService::next(uint8_t current)const{for(uint8_t step=1;step<=SCENE_COUNT;step++){uint8_t candidate=(current+step)%SCENE_COUNT;if(enabled_[candidate])return candidate;}return 6;}
String PlaylistService::toJson()const{JsonDocument d;JsonArray scenes=d["scenes"].to<JsonArray>();for(uint8_t i=0;i<SCENE_COUNT;i++){JsonObject s=scenes.add<JsonObject>();s["id"]=IDS[i];s["enabled"]=enabled_[i];}String out;serializeJson(d,out);return out;}
bool PlaylistService::updateFromJson(const String& json){JsonDocument d;if(deserializeJson(d,json)||!d["scenes"].is<JsonArray>())return false;bool next[SCENE_COUNT]={};for(JsonObject item:d["scenes"].as<JsonArray>()){String id=item["id"]|"";for(uint8_t i=0;i<SCENE_COUNT;i++)if(id==IDS[i])next[i]=item["enabled"]|false;}bool any=false;for(uint8_t i=0;i<SCENE_COUNT;i++){enabled_[i]=next[i];any|=next[i];}if(!any)enabled_[6]=true;save();return true;}
void PlaylistService::save(){uint8_t mask=0;for(uint8_t i=0;i<SCENE_COUNT;i++)if(enabled_[i])mask|=1<<i;store_.putUChar("mask",mask);}
