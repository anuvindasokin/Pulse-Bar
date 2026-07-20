#pragma once
#include <Arduino.h>
#include <FastLED.h>
#include "display/MatrixMapper.h"

class DisplayManager {
 public:
  void begin();
  void setBrightness(uint8_t value);
  uint8_t brightness() const { return brightness_; }
  void showNumber(int64_t value);
  void showClock();
  void showTime(uint64_t ms);
  void showText(const String& text, uint32_t offset = 0);
  void testPattern();
 private:
  void clear();
  void pixel(int x, int y, CRGB color);
  void glyph(char c, int x, CRGB color);
  void present();
  CRGB leds_[256];
  MatrixMapper mapper_;
  uint8_t brightness_ = 48;
  bool hardwareEnabled_ = false;
};
