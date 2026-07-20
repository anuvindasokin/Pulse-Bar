#pragma once
#include <Arduino.h>
class StopwatchService { public: void start(); void pause(); void reset(); uint64_t elapsedMs() const; bool running() const{return running_;} private: bool running_=false; uint32_t started_=0; uint64_t accumulated_=0; };
