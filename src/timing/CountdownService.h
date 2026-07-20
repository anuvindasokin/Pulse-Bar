#pragma once
#include <Arduino.h>
class CountdownService { public: void start(uint32_t seconds); void pause(); void resume(); void reset(); void update(); uint64_t remainingMs()const; bool isActive()const{return active_;} bool running()const{return running_;} private: bool active_=false,running_=false; uint32_t anchor_=0; uint64_t remaining_=0; };
