# OpenRV Thermostat — Amazon.ca Shopping List

Everything you need for the v0.1 breadboard prototype. Total ~$85-110 CAD.

---

## Core Components

### 1. ESP32-S3 Dev Board
**Search:** `ESP32-S3-DevKitC-1`

| Option | Price | Link |
|--------|-------|------|
| XUYUH ESP32-S3-DevKitC-1 (N16R8, 16MB flash) | ~$20.63 | <https://www.amazon.ca/XUYUH-ESP32S3-DevKitC-1-Development-Microcontroller-ESP32S3-WROOM-1-N16R8/dp/B0CXTCZ7SD> |
| ESP32-S3-WROOM-1 16MB flash | ~$24.95 | <https://www.amazon.ca/dp/B0DLBFDL8N> |

*Recommendation: Get the XUYUH one — same board, cheaper, and 16MB flash is plenty.*

---

### 2. 4-Channel Relay Module
**Search:** `4 channel relay module 5V optocoupler`

| Option | Price | Link |
|--------|-------|------|
| 5V/12V/24V 4-Channel Relay Module (get the **12V** version) | ~$11.02 | <https://www.amazon.ca/4-Channel-Module%EF%BC%8C-Optocoupler-Trigger-Arduino/dp/B07S7DH6RN> |
| DIYables 4-Channel 5V Relay | Check price | <https://www.amazon.ca/DIYables-4-Channel-Arduino-ESP8266-Raspberry/dp/B0BXKKYH5C> |

*Note: The 12V relay module is better since you're running on 12V RV power. The 5V version needs the buck converter output. Either works.*

---

### 3. DS18B20 Temperature Sensor
**Search:** `DS18B20 waterproof temperature sensor`

| Option | Price | Link |
|--------|-------|------|
| DROK 5-Pack Waterproof DS18B20 + 4.7kΩ resistors | ~$21.84 | <https://www.amazon.ca/DROK-Waterproof-Temperature-Resistors-Raspberry/dp/B0FLDRGXZD> |
| Gikfun 3-Pack Waterproof DS18B20 | ~$19.38 | <https://www.amazon.ca/dp/B012C597T0> |
| Single waterproof probe 3M cable | ~$8.92 | <https://www.amazon.ca/Stainless-Waterproof-Temperature-Suitable-arduino-3M/dp/B09L713JCD> |

*Recommendation: The DROK 5-pack includes the 4.7kΩ pull-up resistors you need. If you already have resistors, the single probe is fine.*

---

### 4. 12V to 5V Buck Converter
**Search:** `LM2596 buck converter 12V to 5V`

| Option | Price | Link |
|--------|-------|------|
| DROK LM2596 with LED voltmeter (adjustable) | Check price | <https://www.amazon.ca/DROK-Converter-Step-down-Transformer-Stabilizer/dp/B00JUFJ1GA> |
| LM2596 5-pack adjustable | ~$23.99 | <https://www.amazon.ca/FainWan-LM2596S-ADJ-Module-Regulator-Adjustable/dp/B09JZCH3S8> |

*Note: You need to adjust the potentiometer to output 5V before connecting to the ESP32.*

---

### 5. 2.8" TFT Touch Display (ILI9341 + XPT2046)
**Search:** `2.8 inch ILI9341 TFT touch display`

| Option | Price | Link |
|--------|-------|------|
| 2.8" ILI9341 TFT + Touch, SPI, 5V/3.3V | ~$15.29 | <https://www.amazon.ca/Module-ILI9341-240x320-Serial-Compatible/dp/B0DHK5H4SZ> |
| 2.8" ILI9341 with SD slot + stylus | ~$15.78 | <https://www.amazon.ca/Display-240x320-ILI9341-Compatible-Controller/dp/B0BYDPLCMZ> |
| 2.8" ESP32-2432S028R (all-in-one, includes ESP32) | ~$27.98 | <https://www.amazon.ca/dp/B0CG2WQGP9> |

*Recommendation: The first or second option — standalone display you wire to your ESP32. The ESP32-2432S028R is neat but may have different pin mappings.*

---

### 6. Breadboard + Jumper Wires
**Search:** `breadboard jumper wires kit`

| Option | Price | Link |
|--------|-------|------|
| Elegoo 120pcs Dupont wires (M-M, M-F, F-F) | ~$12.99 | <https://www.amazon.ca/dp/B01EV70C78> |
| RGBZONE 120pcs Dupont wires | ~$10.50 | <https://www.amazon.ca/dp/B01M1IEUAF/> |
| Breadboard kit (2 breadboards + 205 wires + tweezer) | ~$19.99 | <https://www.amazon.ca/Breadboards-Include-Solderless-Jumper-Tweezer/dp/B0BMFXPSVG> |

*You probably already have breadboards and jumper wires. If not, the Elegoo kit is solid.*

---

## Optional / Nice to Have

| Item | Why | Search |
|------|-----|--------|
| 4.7kΩ resistor pack | DS18B20 pull-up (if not in sensor kit) | `4.7k ohm resistor 1/4W` |
| USB-C cable | Programming the ESP32-S3 | `USB-C cable data` |
| 12V bench power supply | Testing without RV battery | `12V DC power supply 2A` |
| Multimeter | Verifying voltages | `digital multimeter` |

---

## Minimal v0.1 Build (if you buy only what's needed)

| Item | Est. Price |
|------|-----------|
| ESP32-S3 board | $21 |
| 4-ch relay module | $11 |
| DS18B20 (single) | $9 |
| Buck converter | $13 |
| TFT display | $15 |
| **Subtotal** | **~$69 CAD** |

*Add $10-13 for jumper wires if you don't have them. Total ~$80 CAD for the breadboard prototype.*

---

## What You Already Have
Check your parts bin before ordering:
- [ ] Breadboard(s)
- [ ] Jumper wires (M-M, M-F)
- [ ] USB-C cable for programming
- [ ] 4.7kΩ resistors
- [ ] Any ESP32 dev boards
- [ ] Any relay modules

---

*Last updated: 2026-05-18*
