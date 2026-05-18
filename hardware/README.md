# OpenRV Hardware

## RV Wiring Reference

OpenRV is a drop-in replacement for standard 6-wire RV thermostats. The existing wiring harness in your RV wall provides both power and control signals.

### Wire Mapping

| Color | Function | Direction | Typical Load |
|-------|----------|-----------|--------------|
| **Red** | +12V DC Power | Input | ~50-200mA (thermostat + relays) |
| **Blue** | -12V DC Ground | Input | — |
| **White** | Furnace Control | Output | Relay coil: ~100-200mA |
| **Yellow** | AC Compressor | Output | Relay coil: ~100-300mA |
| **Green** | Fan High Speed | Output | Relay coil: ~100-200mA |
| **Grey** | Fan Low Speed | Output | Relay coil: ~100-200mA |

### How It Works

The thermostat does NOT switch line voltage (120V AC). It is a **low-current control device** that sends switched +12V signals to relays located elsewhere:

- Furnace control board → has its own relay that turns the burner and blower on
- Rooftop AC unit → has a contactor that engages the compressor and fan
- Fan speed relay box → switches between Hi and Lo speed

Our thermostat replicates this: **connect +12V to the appropriate wire when a function is called for.**

### Output Switching Options

There are two practical ways to switch the +12V outputs on the PCB:

#### Option A: Mechanical Relays (Recommended for v0.2)
- **Pros**: Complete galvanic isolation, handles inductive loads well, bulletproof
- **Cons**: Larger, audible click, limited lifetime (~100k cycles)
- **Part suggestion**: Omron G5V-1 or similar 12V SPDT, or automotive micro-relays

#### Option B: High-Side MOSFETs (v0.3+ optimization)
- **Pros**: Silent, smaller, longer lifetime, faster switching
- **Cons**: No isolation, need flyback diodes for inductive loads, more complex driving circuit (P-FET needs gate pulled below 12V)
- **Part suggestion**: P-channel MOSFET like IRF9540 or PMV27UPE with gate driver

### Power Budget (Estimate)

| Component | Current Draw |
|-----------|-------------|
| ESP32-S3 (active + WiFi) | ~80-150mA @ 3.3V → ~25-50mA @ 12V |
| Display (3.5" IPS, 320×240) | ~50-100mA @ 3.3V → ~15-30mA @ 12V |
| 4× Relays (all active) | ~80-200mA each → 320-800mA @ 12V |
| Sensors | ~1-5mA each |
| **Total typical** | **~500mA - 1A @ 12V** |
| **Total worst case** | **~1.2A @ 12V** |

### Voltage Regulators Needed

From 12V input:
1. **Buck converter**: 12V → 5V (for relay coils and ESP32 VIN)
2. **LDO**: 5V → 3.3V (clean power for ESP32, display, sensors)

Or optionally use a dual-output buck converter.

### Protection

- Fuse on +12V input (2A fast-blow automotive fuse)
- Reverse polarity protection (diode or P-FET)
- Transient voltage suppression (TVS diode, protects against load dumps from RV alternator)
- Flyback diodes across relay coils (or at output terminals) if using MOSFETs
