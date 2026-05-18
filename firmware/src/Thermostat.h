#pragma once
#include <Arduino.h>
#include "config.h"

class Thermostat {
public:
    Thermostat();
    
    // Core operations
    void loadSettings();
    void saveSettings();
    void updateTemperature(float tempF);
    void evaluate();
    
    // Setters
    void setMode(ThermoMode mode);
    void setTargetTemp(float tempF);
    void setFanMode(FanMode fan);
    void incrementTarget();
    void decrementTarget();
    
    // Getters
    float getCurrentTemp() const { return currentTemp; }
    float getTargetTemp() const { return targetTemp; }
    ThermoMode getMode() const { return mode; }
    FanMode getFanMode() const { return fanMode; }
    
    // Output states (computed by evaluate())
    bool isFurnaceOn() const { return furnaceOn; }
    bool isCompressorOn() const { return compressorOn; }
    bool isFanHiOn() const { return fanHiOn; }
    bool isFanLoOn() const { return fanLoOn; }
    
    // Status string for logging
    const char* modeString() const;
    
private:
    float currentTemp = NAN;
    float targetTemp = TEMP_DEFAULT_F;
    ThermoMode mode = MODE_OFF;
    FanMode fanMode = FAN_AUTO;
    
    // Computed output states
    bool furnaceOn = false;
    bool compressorOn = false;
    bool fanHiOn = false;
    bool fanLoOn = false;
    
    // EEPROM settings address
    static const int EEPROM_SETTINGS_ADDR = 0;
    static const uint16_t SETTINGS_MAGIC = 0x4F52; // "OR"
    
    struct Settings {
        uint16_t magic;
        float targetTemp;
        uint8_t mode;
        uint8_t fanMode;
        uint16_t crc;
    };
};
