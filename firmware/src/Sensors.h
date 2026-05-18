#pragma once
#include <Arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include "config.h"

class Sensors {
public:
    Sensors();
    void init();
    void read();
    float getTemperatureF() const { return tempF; }
    float getTemperatureC() const { return tempC; }
    bool isValid() const { return valid; }

private:
    OneWire oneWire;
    DallasTemperature ds18b20;
    float tempC = NAN;
    float tempF = NAN;
    bool valid = false;
    uint32_t lastReadSuccess = 0;
};
