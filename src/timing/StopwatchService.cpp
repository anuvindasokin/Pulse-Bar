#include "timing/StopwatchService.h"
void StopwatchService::start(){if(!running_){started_=millis();running_=true;}}
void StopwatchService::pause(){if(running_){accumulated_+=uint32_t(millis()-started_);running_=false;}}
void StopwatchService::reset(){running_=false;accumulated_=0;started_=0;}
uint64_t StopwatchService::elapsedMs()const{return accumulated_+(running_?uint32_t(millis()-started_):0);}
