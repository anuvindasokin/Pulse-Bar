# Hardware

| Function | GPIO | Status |
|---|---:|---|
| WS2812 data | `-1` | Must be selected and validated |
| I²S BCLK | `-1` | Must be selected and validated |
| I²S LRCLK/WS | `-1` | Must be selected and validated |
| I²S data | `-1` | Must be selected and validated |
| Amplifier enable | `-1` | Optional; must be selected |

Use an external regulated 5 V LED supply, common ground, suitable power injection, bulk capacitance, a series data resistor, and a logic-level shifter where required. The firmware deliberately leaves output disabled while pins are unset.
