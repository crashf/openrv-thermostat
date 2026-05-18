# OpenRV Firmware

ESP32-S3 firmware for the OpenRV thermostat. Built with PlatformIO + Arduino framework.

## Quick Start

### Prerequisites

- [PlatformIO](https://platformio.org/install) (VS Code extension or CLI)
- ESP32-S3 development board
- Mini USB-C cable

### Build & Upload

```bash
cd firmware/
pio run --target upload
```

### Flash Filesystem (Web UI)

```bash
pio run --target uploadfs
```

This uploads the files in `data/www/` to the ESP32's SPIFFS filesystem so the web UI is available.

### Monitor Serial Output

```bash
pio device monitor --baud 115200
```

## Pin Mapping

| Pin | Function | Wire Color |
|-----|----------|------------|
| GPIO 5  | Furnace relay | White |
| GPIO 6  | AC Compressor relay | Yellow |
| GPIO 7  | Fan Hi relay | Green |
| GPIO 8  | Fan Lo relay | Grey |
| GPIO 4  | DS18B20 (1-Wire) | — |
| GPIO 2  | I2C SCL | — |
| GPIO 3  | I2C SDA | — |
| GPIO 10 | TFT CS | — |
| GPIO 11 | TFT DC | — |
| GPIO 12 | TFT RST | — |
| GPIO 13 | TFT MOSI | — |
| GPIO 14 | TFT MISO | — |
| GPIO 15 | TFT SCK | — |
| GPIO 16 | Touch CS | — |
| GPIO 17 | Touch IRQ | — |

## Architecture

```
main.cpp
├── Thermostat    — Control logic (modes, hysteresis, safety)
├── Sensors       — DS18B20 temperature reading + expansion I2C
├── Relays        — GPIO output control with safety interlocks
├── Display       — TFT_eSPI touch UI
├── WebServer     — AsyncWebServer with REST API + WebSocket
└── WiFiManager   — STA/AP connectivity
```

## Configuration

Edit `src/config.h` before building:

- **WiFi credentials** — Set `WIFI_STA_SSID` and `WIFI_STA_PASSWORD`
- **Pin assignments** — Adjust if your dev board uses different GPIOs
- **Temperature limits** — `TEMP_MIN_F` / `TEMP_MAX_F` / `TEMP_HYSTERESIS_F`

## Web UI

After connecting to WiFi, the thermostat serves a web UI at:

```
http://openrv-thermostat.local/     (mDNS)
http://<esp-ip>/                     (direct IP)
```

Features:
- Real-time temperature display (WebSocket)
- Setpoint adjustment with ± buttons
- Mode selection (Off / Fan Lo / Fan Hi / Cool / Heat / Auto)
- Live status of all 4 relay outputs

## REST API

| Endpoint | Method | Description |
|----------|--------|-------------|
| `/api/status` | GET | Full system state (JSON) |
| `/api/mode` | POST | Set mode: `{"mode": 0-5}` |
| `/api/target` | POST | Set target: `{"target": 72.0}` |

## Modes

| Mode | Value | Behavior |
|------|-------|----------|
| OFF | 0 | All outputs disabled |
| FAN LO | 1 | Fan Lo relay on |
| FAN HI | 2 | Fan Hi relay on |
| COOL | 3 | Compressor + Fan Hi when temp > target + hysteresis |
| HEAT | 4 | Furnace + Fan Hi when temp < target - hysteresis |
| AUTO | 5 | Automatically chooses heat or cool based on deviation |

## Safety

- **Furnace + Compressor interlock** — They can never both be on at the same time (enforced in `Relays.cpp`)
- **EEPROM persistence** — Settings survive reboot
- **Emergency off** — Call `relays.emergencyOff()` from any safety condition
- **Sensor validation** — Refuses to control outputs without valid temperature reading

## OTA Updates

Enable OTA by uncommenting `upload_protocol = espota` in `platformio.ini` and setting `upload_port` to the device's IP.

Then build with:
```bash
pio run --target upload
```
