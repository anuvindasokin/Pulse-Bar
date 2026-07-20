#include "timing/CountdownService.h"
void CountdownService::start(uint32_t s){remaining_=uint64_t(s)*1000;anchor_=millis();active_=running_=remaining_>0;}
void CountdownService::pause(){if(running_){remaining_=remainingMs();running_=false;}}
void CountdownService::resume(){if(active_&&!running_){anchor_=millis();running_=true;}}
void CountdownService::reset(){active_=running_=false;remaining_=0;}
uint64_t CountdownService::remainingMs()const{if(!running_)return remaining_;uint32_t e=millis()-anchor_;return e>=remaining_?0:remaining_-e;}
void CountdownService::update(){if(running_&&remainingMs()==0){remaining_=0;active_=running_=false;}}
