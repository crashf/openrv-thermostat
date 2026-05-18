# OpenRV Thermostat — Build Guide

## v0.1 Breadboard Prototype

### Parts Needed

| Part | Qty | Source | Notes |
|------|-----|--------|-------|
| ESP32-S3-DevKitC-1 | 1 | Amazon / Adafruit | Any ESP32-S3 dev board |
| 2-channel relay module | 1 | Amazon | 5V logic, optoisolated |
| DS18B20 temp sensor | 1-2 | Amazon | Waterproof version recommended |
| 4.7kΩ resistor | 1 | Any | DS18B20 pull-up |
| 12V to 5V buck converter | 1 | Amazon | LM2596 or similar |
| Jumper wires | ~20 | Any | M-M and M-F |
| Breadboard | 1 | Any | Half-size or full |
| 12V DC power supply | 1 | Bench PSU | RV battery (~12.6V) or regulated supply |

### Wiring Diagram (v0.1)

```
ESP32-S3          Relay Module          DS18B20 (HVAC)     DS18B20 (Ambient)
─────────         ───────────           ───────────        ───────────────
3.3V              VCC (via 5V buck)     Red (VDD)          Red (VDD)
GND               GND                   Black (GND)        Black (GND)
GPIO 18           IN1 (Cool)            ─                  ─
GPIO 19           IN2 (Heat)            ─                  ─
GPIO 4            ─                     Yellow (Data)        Yellow (Data)
                                                                      |
                                                                     4.7kΩ
                                                                      |
                                                                     3.3V
```

**Note**: The ESP32-S3 is 3.3V logic. Some relay modules trigger on 3.3V, others need 5V.
If your relay doesn't trigger at 3.3V, use a logic level shifter or a transistor buffer.

### Software Setup

1. Install [PlatformIO](https://platformio.org/install) (VS Code extension recommended)
2. Clone this repo: `git clone https://github.com/wayneledrew/openrv-thermostat.git`
3. Open `firmware/` in PlatformIO
4. Copy `config.example.h` to `config.h` and fill in your WiFi credentials
5. Build and upload to ESP32-S3

### Web UI

Once flashed and connected to WiFi:
- Find the ESP32 IP (via serial monitor or router)
- Navigate to `http://<ip>/`
- Basic controls: setpoint up/down, mode (Off/Cool/Heat/Fan), current temp display

### Next Steps

See `docs/flashing.md` for OTA (over-the-air) update instructions once the prototype is working.

---

**⚠️ Safety Note**: This is a prototype. Do not leave unattended. HVAC systems carry mains voltage (120V AC). Use this for low-voltage testing only until the full isolation design is complete.
