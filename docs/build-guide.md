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

### Understanding Your RV Wiring

Before building, you need to know how your RV's existing thermostat works:

**Power wires:**
- **Red** = +12V DC (system power in)
- **Blue** = -12V DC (ground)

**All 4 control outputs are "switched +12V" signals:**
When the thermostat wants to turn on something, it connects +12V to that wire. When off, the wire is floating (open circuit). The thermostat does NOT switch line voltage — it just sends control signals to relays located in the furnace, AC unit, or a separate relay panel.

| Wire | Function | Typical Load |
|------|----------|--------------|
| White | Furnace | Furnace control relay coil (~100-200mA) |
| Yellow | AC Compressor | Rooftop AC contactor coil (~100-300mA) |
| Green | Fan Hi | High-speed fan relay (~100-200mA) |
| Grey | Fan Low | Low-speed fan relay (~100-200mA) |

Each output only needs to source ~0.5A max. That's easily handled by a small relay or transistor on our board.

### Wiring Diagram (v0.1)

```
ESP32-S3          Relay Module (4-ch)        DS18B20 Sensors
─────────         ─────────────────           ───────────────
3.3V ──────────── VCC (via 5V buck)          Red (VDD) ────────┬────── 3.3V
GND  ──────────── GND                        Black (GND) ────────┼────── GND
GPIO 18 ───────── IN1 (Furnace / White)      Yellow (Data) ────┼────── GPIO 4
GPIO 19 ───────── IN2 (Compressor / Yellow)                      │       |
GPIO 20 ───────── IN3 (Fan Hi / Green)     4.7kΩ pull-up ──────┘       |
GPIO 21 ───────── IN4 (Fan Low / Grey)                                 |
                                                                       4.7kΩ
                                                                        |
                                                                       3.3V

Power Input:
Red (+12V) ────── Buck Converter VIN ────── 5V out ───── ESP32-S3 5V pin
Blue (GND) ────── Buck Converter GND ────── GND

Relay Outputs (to RV wiring harness):
COM (relay module) ─── +12V from Red wire
NO1 ──────────────── White (Furnace)
NO2 ──────────────── Yellow (Compressor)
NO3 ──────────────── Green (Fan Hi)
NO4 ──────────────── Grey (Fan Low)
```

**Note**: In v0.1 breadboard, use a 4-channel relay module. The COM pin on the relay module connects to +12V, and each NO (normally open) output connects to the corresponding RV wire. When the relay activates, it connects +12V to that wire — exactly what the factory thermostat does.

**Note**: The ESP32-S3 is 3.3V logic. Most 5V relay modules trigger fine at 3.3V, but if yours doesn't, add a logic level shifter or use a 3.3V relay module.

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
