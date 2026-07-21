#pragma once

#include <Arduino.h>
#include "display/DisplayManager.h"
#include "display/PlaylistService.h"
#include "network/WiFiService.h"
#include "timing/CountdownService.h"
#include "timing/StopwatchService.h"
#include "web/WebServerService.h"

class AppController {
 public:
  void begin();
  void loop();

 private:
  DisplayManager display_;
  WiFiService wifi_;
  StopwatchService stopwatch_;
  CountdownService timer_;
  PlaylistService playlist_;
  WebServerService web_{display_, wifi_, stopwatch_, timer_, playlist_};
  uint32_t lastFrameAt_ = 0;
  uint32_t sceneStartedAt_ = 0;
  uint32_t metricsUpdatedAt_ = 0;
  uint8_t scene_ = 0;
  bool switchWasDown_ = false;
};
