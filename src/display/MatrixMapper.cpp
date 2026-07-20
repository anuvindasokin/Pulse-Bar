#include "display/MatrixMapper.h"

uint16_t MatrixMapper::map(uint8_t x, uint8_t y) const {
  if (x >= 32 || y >= 8) return UINT16_MAX;
  if (config_.mirrorX) x = 31 - x;
  if (config_.mirrorY) y = 7 - y;
  uint8_t matrix = x / 8, lx = x % 8, ly = y;
  const uint8_t r = config_.rotation[matrix] % 4;
  for (uint8_t i = 0; i < r; ++i) { uint8_t old = lx; lx = 7 - ly; ly = old; }
  if (config_.firstPixel == PixelCorner::TopRight || config_.firstPixel == PixelCorner::BottomRight) lx = 7 - lx;
  if (config_.firstPixel == PixelCorner::BottomLeft || config_.firstPixel == PixelCorner::BottomRight) ly = 7 - ly;
  if (config_.reverseOrder) matrix = 3 - matrix;
  const uint8_t within = config_.serpentine && (ly & 1) ? ly * 8 + (7 - lx) : ly * 8 + lx;
  return matrix * 64 + within;
}
