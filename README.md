# 🌡️ OpenRV Thermostat

An open-source, open-hardware smart thermostat designed specifically for RVs — because factory thermostats are terrible and internet clouds shouldn't control your AC.

## Why?

- **No cloud dependency** — Everything runs local. No internet required.
- **Works with your inverter** — Understands DC battery power vs. shore power vs. generator.
- **Multiple zones** — Control front + rear independently, because one temperature sensor in a 30ft box is a joke.
- **Home Assistant ready** — Native integration, no weird bridges.
- **Open everything** — Schematics, firmware, and software are fully documented and modifiable.

## Hardware Specs (Target)

| Feature | Spec |
|---------|------|
| MCU | ESP32-S3 (WiFi + Bluetooth, dual core) |
| Display | 320×240 IPS touchscreen (2.8" or 3.5") |
| Temp Sensors | 1× onboard, up to 4× remote DS18B20 or SHT30 |
| Relays | 2× 5A HVAC relays (heat/cool/fan control) |
| Power | 12V DC input (9-16V range), onboard buck converter |
| Connectivity | WiFi, Bluetooth for mobile setup |
| Enclosure | 3D printable, fits standard RV wall cutout |

## Repository Structure

```
├── hardware/
│   ├── pcb/              — KiCad schematics and PCB layout
│   ├── enclosure/        — 3D printable case (FreeCAD / STEP / STL)
│   └── bom/              — Bill of Materials with LCSC/JLCPCB part numbers
├── firmware/
│   ├── src/              — ESP-IDF / Arduino / PlatformIO firmware
│   ├── lib/              — Custom libraries
│   └── tests/            — Hardware-in-the-loop tests
├── software/
│   ├── backend/          — Optional local server (ESP32 runs standalone)
│   ├── frontend/         — Web UI for configuration
│   └── home-assistant-custom-component/
│                         — Native HA integration
├── docs/
│   ├── build-guide.md    — Full assembly instructions
│   ├── flashing.md       — How to flash firmware
│   └── api.md            — Local REST + WebSocket API docs
├── scripts/              — Build, flash, and test automation
└── .github/workflows/    — CI for firmware builds, PCB DRC checks
```

## Quick Start (Developer Preview)

This project is in early active development. The first milestone is a breadboard prototype with:
- ESP32 dev board
- Relay module
- DS18B20 temperature sensor
- Web UI for basic control

See `docs/build-guide.md` for current prototype instructions.

## License

- **Hardware**: [CERN-OHL-S-2.0](https://ohwr.org/cern_ohl_s_v2.txt) — Free to use, modify, and manufacture.
- **Firmware/Software**: [MIT License](LICENSE) — Free to use in any project, commercial or personal.

## Roadmap

- [ ] v0.1 — Breadboard prototype with ESP32 + relay + web UI
- [ ] v0.2 — PCB design (KiCad), 3D printed enclosure v1
- [ ] v0.3 — Multi-zone support (remote temp sensors)
- [ ] v0.4 — Home Assistant custom component
- [ ] v0.5 — Inverter-aware power modes (DC battery vs. shore vs. generator)
- [ ] v1.0 — Full hardware kit with assembly guide

## Contributing

Open issues, submit PRs, or fork for your own RV. This is built for the community.

## Credits & Inspiration

Built for life on the road. 🚐
