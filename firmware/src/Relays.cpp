#include "Relays.h"

void Relays::init() {
    pinMode(PIN_FURNACE, OUTPUT);
    pinMode(PIN_AC_COMPRESSOR, OUTPUT);
    pinMode(PIN_FAN_HI, OUTPUT);
    pinMode(PIN_FAN_LO, OUTPUT);
    
    // Default all off
    digitalWrite(PIN_FURNACE, LOW);
    digitalWrite(PIN_AC_COMPRESSOR, LOW);
    digitalWrite(PIN_FAN_HI, LOW);
    digitalWrite(PIN_FAN_LO, LOW);
    
    Serial.println("[Relays] Initialized, all outputs OFF");
}

void Relays::setFurnace(bool on) {
    if (furnaceOn != on) {
        furnaceOn = on;
        digitalWrite(PIN_FURNACE, on ? HIGH : LOW);
        Serial.printf("[Relays] Furnace %s\n", on ? "ON" : "OFF");
    }
}

void Relays::setCompressor(bool on) {
    if (compressorOn != on) {
        compressorOn = on;
        digitalWrite(PIN_AC_COMPRESSOR, on ? HIGH : LOW);
        Serial.printf("[Relays] Compressor %s\n", on ? "ON" : "OFF");
    }
}

void Relays::setFanHi(bool on) {
    if (fanHiOn != on) {
        fanHiOn = on;
        digitalWrite(PIN_FAN_HI, on ? HIGH : LOW);
        Serial.printf("[Relays] Fan Hi %s\n", on ? "ON" : "OFF");
    }
}

void Relays::setFanLo(bool on) {
    if (fanLoOn != on) {
        fanLoOn = on;
        digitalWrite(PIN_FAN_LO, on ? HIGH : LOW);
        Serial.printf("[Relays] Fan Lo %s\n", on ? "ON" : "OFF");
    }
}

void Relays::emergencyOff() {
    setFurnace(false);
    setCompressor(false);
    setFanHi(false);
    setFanLo(false);
    Serial.println("[Relays] EMERGENCY OFF — all outputs disabled");
}
