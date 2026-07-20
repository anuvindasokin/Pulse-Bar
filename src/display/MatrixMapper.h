#pragma once
#include <Arduino.h>

enum class PixelCorner : uint8_t { TopLeft, TopRight, BottomLeft, BottomRight };

struct MatrixConfig {
  bool serpentine = true;
  bool mirrorX = false;
  bool mirrorY = false;
  bool reverseOrder = false;
  PixelCorner firstPixel = PixelCorner::TopLeft;
  uint8_t rotation[4] = {0, 0, 0, 0};
};

class MatrixMapper {
 public:
  explicit MatrixMapper(const MatrixConfig& config = {}) : config_(config) {}
  uint16_t map(uint8_t x, uint8_t y) const;
  void setConfig(const MatrixConfig& config) { config_ = config; }
 private:
  MatrixConfig config_;
};
