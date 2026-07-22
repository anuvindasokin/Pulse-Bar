# PulseBar

PulseBar is an ESP32-S3 desktop social counter and 32×8 RGB information display. This repository currently contains the first runnable MVP: a responsive local dashboard, setup access point, REST controls, configurable matrix mapper, display scenes, stopwatch, countdown timer, clock, and mock counter.

## Hardware safety

GPIO values intentionally default to `-1` in `include/BoardConfig.h`. Confirm your exact XIAO ESP32-S3 wiring before enabling LED or I²S output. Power the 256 LEDs from a separate regulated 5 V supply with a common ground; never power the matrices from the XIAO.

## Build

1. Install [PlatformIO](https://platformio.org/).
2. Set and document validated pins in `include/BoardConfig.h`.
3. Build with `pio run`.
4. Upload with `pio run -t upload`.
5. Open the serial monitor with `pio device monitor`.

## Open the interface on a PC

Run the desktop simulator from PowerShell:

```powershell
.\preview.ps1
```

It opens `http://127.0.0.1:8080` and emulates the device API, including the stopwatch, timer, brightness, Wi-Fi scan, and setup flow. The preview reads the exact embedded interface from `src/web/WebUi.h` on every browser refresh. There is no duplicate web page to keep synchronized: firmware interface changes are therefore reflected in the desktop page automatically.

## Share the interface publicly

`render-demo.yaml` deploys the same simulator as a public, mobile-friendly Render web service. In Render, create a new Blueprint and select `render-demo.yaml` from this repository. Render will provide a shareable `https://pulsebar-interface-demo.onrender.com`-style URL after the first deploy.

The public demo serves the interface directly from `src/web/WebUi.h`, just like the firmware and desktop preview. Its controls use simulated data and do not connect to or change a physical PulseBar. The free Render service may take a short time to wake after it has been idle.

On first boot PulseBar immediately starts the `PulseBar-Setup` access point. Connect with a phone or computer; its captive setup page should open automatically. If it does not, browse to `http://192.168.4.1`. Choose a network, enter its password, name the device, and choose a time zone. After it connects, use `http://pulsebar.local` on the same network.

## API

See [Social API setup](docs/social-api-setup.md) for customer credential instructions. The same guide is available inside the account connection interface.

Facebook and Instagram use the included multi-customer [PulseBar Connect backend](cloud/README.md), deployable on Render through `render.yaml`. It provides customer login, one-time device claiming, PostgreSQL ownership records, Page selection, and encrypted social connections while keeping Meta credentials out of ESP32 firmware.

- `GET /api/v1/status` returns live device, network, stopwatch, and timer state.
- `POST /api/v1/control` accepts an action such as `stopwatchStart`, `timerStart`, `brightness`, or `displayTest`.
- `POST /api/v1/connect/config` stores the installer-provided PulseBar Connect URL and per-device credentials.
- `POST /api/v1/connect/link` creates a short-lived Meta authorization link through the backend.
- `GET /api/v1/metrics` refreshes normalized Facebook and Instagram counters.

All responses use `{ "success": boolean, "data": ..., "error": ... }`.

## Roadmap

Next increments add persisted configuration and Wi-Fi onboarding, playlist editing, audio, official-provider adapters, notifications, authentication, and OTA. No secrets belong in this repository.
