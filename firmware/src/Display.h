#pragma once
#include <Arduino.h>
#include <TFT_eSPI.h>
#include "Thermostat.h"
#include "Sensors.h"

class Display {
public:
    Display();
    void init();
    void update(Thermostat& thermostat, Sensors& sensors);
    void handleTouch(Thermostat& thermostat);
    
private:
    TFT_eSPI tft;
    bool initialized = false;
    float lastTemp = NAN;
    ThermoMode lastMode = MODE_OFF;
    float lastTarget = NAN;
    
    void drawBackground();
    void drawTemperature(float tempF);
    void drawTarget(float targetF);
    void drawMode(ThermoMode mode);
    void drawOutputs(Thermostat& thermostat);
    void drawWiFiStatus();
    void drawTouchButtons();
    
    // Touch zones
    struct TouchZone {
        int x, y, w, h;
        void (*callback)(Thermostat&, int);
        int value;
    };
    static const int MAX_ZONES = 10;
    TouchZone zones[MAX_ZONES];
    int zoneCount = 0;
    unsigned long lastTouchTime = 0;
};
