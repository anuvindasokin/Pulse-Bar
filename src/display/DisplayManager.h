#pragma once
#include <Arduino.h>
#include <FastLED.h>
#include "display/MatrixMapper.h"

class DisplayManager {
 public:
  void begin();
  void setBrightness(uint8_t value);
  uint8_t brightness() const { return brightness_; }
  void setColor(uint32_t value) { color_ = CRGB(value); }
  uint32_t color() const { return (uint32_t(color_.r)<<16)|(uint32_t(color_.g)<<8)|color_.b; }
  void showNumber(int64_t value);
  void showNumber(int64_t value, CRGB color);
  void showNumberGradient(int64_t value, CRGB start, CRGB middle, CRGB end);
  void showMetric(int64_t value, bool views, CRGB color);
  void showMetricGradient(int64_t value, bool views, CRGB start, CRGB middle, CRGB end);
  void showClock(bool use24Hour = true);
  void showTime(uint64_t ms);
  void showText(const String& text, uint32_t offset = 0);
  void testPattern();
 private:
  void clear();
  void pixel(int x, int y, CRGB color);
  void glyph(char c, int x, CRGB color);
  void metricIcon(bool views, CRGB color);
  void present();
  CRGB leds_[256];
  MatrixMapper mapper_;
  uint8_t brightness_ = 48;
  bool hardwareEnabled_ = false;
  CRGB color_ = CRGB(255, 77, 145);
};
