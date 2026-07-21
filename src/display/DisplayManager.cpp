#include "display/DisplayManager.h"
#include "BoardConfig.h"
#include <time.h>

static const uint8_t DIGITS[10][5] = {{7,5,5,5,7},{2,6,2,2,7},{7,1,7,4,7},{7,1,7,1,7},{5,5,7,1,1},{7,4,7,1,7},{7,4,7,5,7},{7,1,1,1,1},{7,5,7,5,7},{7,5,7,1,7}};

void DisplayManager::begin() {
  if (PulseBarPins::LED_DATA < 0) { Serial.println("[display] LED_DATA unset; preview-only mode"); return; }
  // FastLED requires a compile-time pin. Set LED_DATA in BoardConfig then mirror it here when wiring is finalized.
  Serial.println("[display] Set a concrete FastLED pin after hardware pin validation");
}
void DisplayManager::setBrightness(uint8_t v) { brightness_ = v; if (hardwareEnabled_) FastLED.setBrightness(v); }
void DisplayManager::clear() { fill_solid(leds_, 256, CRGB::Black); }
void DisplayManager::pixel(int x,int y,CRGB c){ if(x<0||x>31||y<0||y>7)return; auto i=mapper_.map(x,y); if(i<256)leds_[i]=c; }
void DisplayManager::present(){ if(hardwareEnabled_) FastLED.show(); }
void DisplayManager::glyph(char c,int x,CRGB color){ if(c<'0'||c>'9')return; auto& g=DIGITS[c-'0']; for(int y=0;y<5;y++)for(int b=0;b<3;b++)if(g[y]&(1<<(2-b)))pixel(x+b,y+1,color); }
void DisplayManager::showNumber(int64_t v){ clear(); String s=String(v); int x=max(0,(32-(int)s.length()*4+1)/2); for(char c:s){glyph(c,x,color_);x+=4;}present(); }
void DisplayManager::showNumber(int64_t v,CRGB color){clear();String s=String(v);int x=max(0,(32-(int)s.length()*4+1)/2);for(char c:s){glyph(c,x,color);x+=4;}present();}
void DisplayManager::showNumberGradient(int64_t v,CRGB start,CRGB middle,CRGB end){clear();String s=String(v);int x=max(0,(32-(int)s.length()*4+1)/2),count=s.length();for(int i=0;i<count;i++){uint8_t pos=count<2?0:(uint8_t)(i*255/(count-1));CRGB color=pos<128?blend(start,middle,pos*2):blend(middle,end,(pos-128)*2);glyph(s[i],x,color);x+=4;}present();}
void DisplayManager::metricIcon(bool views,CRGB color){static const uint8_t eye[3]={0b01110,0b10101,0b01110};static const uint8_t person[7]={0b010,0b111,0b111,0b010,0b010,0b111,0b111};const uint8_t* rows=views?eye:person;int height=views?3:7,width=views?5:3,x0=views?0:29,y0=views?2:0;for(int y=0;y<height;y++)for(int x=0;x<width;x++)if(rows[y]&(1<<(width-1-x)))pixel(x0+x,y0+y,color);}
void DisplayManager::showMetric(int64_t v,bool views,CRGB color){clear();String s=String(v);if(s.length()>5)s=s.substring(0,5);int areaStart=views?6:0,areaWidth=views?26:28,x=areaStart+max(0,(areaWidth-(int)s.length()*4+1)/2);for(char c:s){glyph(c,x,color);x+=4;}metricIcon(views,color);present();}
void DisplayManager::showMetricGradient(int64_t v,bool views,CRGB start,CRGB middle,CRGB end){clear();String s=String(v);if(s.length()>5)s=s.substring(0,5);int areaStart=views?6:0,areaWidth=views?26:28,x=areaStart+max(0,(areaWidth-(int)s.length()*4+1)/2),count=s.length();for(int i=0;i<count;i++){uint8_t pos=count<2?0:(uint8_t)(i*255/(count-1));CRGB color=pos<128?blend(start,middle,pos*2):blend(middle,end,(pos-128)*2);glyph(s[i],x,color);x+=4;}metricIcon(views,views?start:end);present();}
void DisplayManager::showTime(uint64_t ms){ uint32_t sec=ms/1000; char b[9]; snprintf(b,sizeof(b),"%02lu:%02lu",(unsigned long)(sec/60),(unsigned long)(sec%60)); showText(b,0); }
void DisplayManager::showClock(bool use24Hour){ struct tm t; if(!getLocalTime(&t,0)){showText("--:--",0);return;} char b[6]; strftime(b,sizeof(b),use24Hour?"%H:%M":"%I:%M",&t); showText(b,0); }
void DisplayManager::showText(const String& text,uint32_t offset){ clear(); int width=text.length()*4; int x=width<=32?(32-width)/2:32-(offset%(width+32)); for(char c:text){ if(c>='0'&&c<='9')glyph(c,x,color_); else { uint8_t seed=(uint8_t)c; for(int y=1;y<6;y++)for(int b=0;b<3;b++)if((seed>>(b+(y&1)))&1)pixel(x+b,y,color_); } x+=4;} present(); }
void DisplayManager::testPattern(){ clear(); for(int x=0;x<32;x++)for(int y=0;y<8;y++)pixel(x,y,CHSV(x*8,255,120));present(); }
