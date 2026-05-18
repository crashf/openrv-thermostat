#include "Thermostat.h"
#include <EEPROM.h>

Thermostat::Thermostat() {}

void Thermostat::loadSettings() {
    Settings s;
    EEPROM.get(EEPROM_SETTINGS_ADDR, s);
    
    if (s.magic == SETTINGS_MAGIC) {
        targetTemp = s.targetTemp;
        mode = static_cast<ThermoMode>(s.mode);
        fanMode = static_cast<FanMode>(s.fanMode);
        
        // Validate
        if (targetTemp < TEMP_MIN_F || targetTemp > TEMP_MAX_F) {
            targetTemp = TEMP_DEFAULT_F;
        }
        
        Serial.printf("Loaded settings: target=%.1f°F, mode=%d, fan=%d\n", 
            targetTemp, mode, fanMode);
    } else {
        Serial.println("No saved settings found, using defaults");
        targetTemp = TEMP_DEFAULT_F;
        mode = MODE_OFF;
        fanMode = FAN_AUTO;
        saveSettings();
    }
}

void Thermostat::saveSettings() {
    Settings s;
    s.magic = SETTINGS_MAGIC;
    s.targetTemp = targetTemp;
    s.mode = static_cast<uint8_t>(mode);
    s.fanMode = static_cast<uint8_t>(fanMode);
    
    EEPROM.put(EEPROM_SETTINGS_ADDR, s);
    Serial.println("Settings saved to EEPROM");
}

void Thermostat::updateTemperature(float tempF) {
    currentTemp = tempF;
}

void Thermostat::setMode(ThermoMode newMode) {
    if (mode != newMode) {
        mode = newMode;
        // Force re-evaluation immediately
        evaluate();
        saveSettings();
    }
}

void Thermostat::setTargetTemp(float tempF) {
    tempF = constrain(tempF, TEMP_MIN_F, TEMP_MAX_F);
    if (targetTemp != tempF) {
        targetTemp = tempF;
        saveSettings();
    }
}

void Thermostat::setFanMode(FanMode fan) {
    if (fanMode != fan) {
        fanMode = fan;
        evaluate();
        saveSettings();
    }
}

void Thermostat::incrementTarget() {
    setTargetTemp(targetTemp + 1.0f);
}

void Thermostat::decrementTarget() {
    setTargetTemp(targetTemp - 1.0f);
}

void Thermostat::evaluate() {
    // Default: all outputs off
    furnaceOn = false;
    compressorOn = false;
    fanHiOn = false;
    fanLoOn = false;
    
    if (isnan(currentTemp)) {
        return; // Can't evaluate without temperature reading
    }
    
    float diff = currentTemp - targetTemp; // >0 means too hot, <0 means too cold
    float absDiff = fabs(diff);
    
    switch (mode) {
        case MODE_OFF:
            // All outputs already false
            break;
            
        case MODE_COOL:
            // Turn on AC if too hot (with hysteresis)
            if (diff >= TEMP_HYSTERESIS_F) {
                compressorOn = true;
                fanHiOn = true; // AC always runs fan
            } else if (diff < -TEMP_HYSTERESIS_F) {
                // Turned off, stay off until diff > +hysteresis
            } else {
                // In deadband — maintain previous state to prevent rapid cycling
                // (simplified: off for now, could be smarter)
            }
            break;
            
        case MODE_HEAT:
            // Turn on furnace if too cold
            if (diff <= -TEMP_HYSTERESIS_F) {
                furnaceOn = true;
                fanHiOn = true; // Furnace usually needs fan
            } else if (diff > TEMP_HYSTERESIS_F) {
                // Turned off, stay off until diff < -hysteresis
            }
            break;
            
        case MODE_AUTO:
            // Smart: choose heat or cool based on how far we are
            if (diff >= TEMP_HYSTERESIS_F) {
                compressorOn = true;
                fanHiOn = true;
            } else if (diff <= -TEMP_HYSTERESIS_F) {
                furnaceOn = true;
                fanHiOn = true;
            }
            break;
            
        case MODE_FAN_LO:
            fanLoOn = true;
            break;
            
        case MODE_FAN_HI:
            fanHiOn = true;
            break;
    }
    
    // Override fan based on fanMode setting
    if (fanMode == FAN_LO && !compressorOn && !furnaceOn) {
        fanLoOn = true;
        fanHiOn = false;
    } else if (fanMode == FAN_HI && !compressorOn && !furnaceOn) {
        fanHiOn = true;
        fanLoOn = false;
    }
    // FAN_AUTO: fan only runs with compressor or furnace (already set above)
}

const char* Thermostat::modeString() const {
    switch (mode) {
        case MODE_OFF:     return "OFF";
        case MODE_FAN_LO:  return "FAN LO";
        case MODE_FAN_HI: return "FAN HI";
        case MODE_COOL:    return "COOL";
        case MODE_HEAT:    return "HEAT";
        case MODE_AUTO:    return "AUTO";
        default:           return "UNKNOWN";
    }
}
