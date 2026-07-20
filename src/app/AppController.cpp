#include "app/AppController.h"
#include <LittleFS.h>

void AppController::begin() {
  Serial.println("[PulseBar] booting");
  if (!LittleFS.begin(true)) Serial.println("[PulseBar] LittleFS mount failed");
  display_.begin();
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
  if (now - lastFrameAt_ < 40) return;
  lastFrameAt_ = now;
  if (now - sceneStartedAt_ > 8000) { scene_ = (scene_ + 1) % 3; sceneStartedAt_ = now; }
  if (scene_ == 0) display_.showNumber(12480 + now / 10000);
  else if (scene_ == 1) display_.showClock();
  else if (timer_.isActive()) display_.showTime(timer_.remainingMs());
  else display_.showText("PULSEBAR", now / 80);
}
