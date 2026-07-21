#include "app/AppController.h"
#include <LittleFS.h>
#include "BoardConfig.h"

void AppController::begin() {
  Serial.println("[PulseBar] booting");
  if (!LittleFS.begin(true)) Serial.println("[PulseBar] LittleFS mount failed");
  display_.begin();
  playlist_.begin();
  if(PulseBarPins::STOPWATCH_SWITCH>=0)pinMode(PulseBarPins::STOPWATCH_SWITCH,INPUT_PULLUP);
  wifi_.begin();
  web_.begin();
  sceneStartedAt_ = millis();
  Serial.println("[PulseBar] ready");
}

void AppController::loop() {
  wifi_.loop();
  web_.loop();
  timer_.update();
  const uint32_t now = millis();
  if(PulseBarPins::STOPWATCH_SWITCH>=0){bool down=digitalRead(PulseBarPins::STOPWATCH_SWITCH)==LOW;if(down&&!switchWasDown_){if(stopwatch_.running())stopwatch_.pause();else stopwatch_.start();}switchWasDown_=down;}
  if (now - lastFrameAt_ < 40) return;
  lastFrameAt_ = now;
  if(stopwatch_.running()){display_.showTime(stopwatch_.elapsedMs());return;}
  if(timer_.isActive()){display_.showTime(timer_.remainingMs());return;}
  if(now-sceneStartedAt_>uint32_t(playlist_.durationSeconds())*1000){scene_=playlist_.next(scene_);sceneStartedAt_=now;}
  if(!playlist_.enabled(scene_))scene_=playlist_.next(scene_);
  if(scene_==6)display_.showClock(playlist_.use24Hour());
  else {static const int64_t base[6]={12480,2384000,8320,914000,21600,510000};int64_t value=base[scene_]+now/10000;bool views=scene_%2==1;if(scene_<2)display_.showMetric(value,views,CRGB(255,0,51));else if(scene_<4)display_.showMetric(value,views,CRGB(24,119,242));else display_.showMetricGradient(value,views,CRGB(255,45,149),CRGB(174,52,255),CRGB(92,45,145));}
}
