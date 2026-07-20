#pragma once

namespace PulseBarPins {
constexpr int LED_DATA = -1;
constexpr int I2S_BCLK = -1;
constexpr int I2S_LRCLK = -1;
constexpr int I2S_DATA = -1;
constexpr int AMP_ENABLE = -1;
// Momentary button to GND. Leave -1 until the final PCB GPIO is confirmed.
constexpr int STOPWATCH_SWITCH = -1;
}  // namespace PulseBarPins
