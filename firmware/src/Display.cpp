#include "Display.h"
#include <SPI.h>

Display::Display() : tft() {}

void Display::init() {
    tft.init();
    tft.setRotation(TFT_ROTATION);
    tft.fillScreen(TFT_BLACK);
    
    initialized = true;
    Serial.println("[Display] TFT initialized");
}

void Display::update(Thermostat& thermostat, Sensors& sensors) {
    if (!initialized) return;
    
    float tempF = sensors.getTemperatureF();
    float target = thermostat.getTargetTemp();
    ThermoMode mode = thermostat.getMode();
    
    // Only redraw changed elements to reduce flicker
    if (isnan(tempF) || tempF != lastTemp) {
        drawTemperature(tempF);
        lastTemp = tempF;
    }
    
    if (target != lastTarget || mode != lastMode) {
        drawTarget(target);
        drawMode(mode);
        lastTarget = target;
        lastMode = mode;
    }
    
    drawOutputs(thermostat);
    drawTouchButtons();
}

void Display::handleTouch(Thermostat& thermostat) {
    if (!initialized) return;
    
    uint16_t t_x = 0, t_y = 0;
    bool touched = tft.getTouch(&t_x, &t_y);
    
    if (touched && millis() - lastTouchTime > 200) {
        lastTouchTime = millis();
        for (int i = 0; i < zoneCount; i++) {
            TouchZone& z = zones[i];
            if (t_x >= z.x && t_x <= z.x + z.w && t_y >= z.y && t_y <= z.y + z.h) {
                z.callback(thermostat, z.value);
                break;
            }
        }
    }
}

void Display::drawTemperature(float tempF) {
    tft.setTextSize(1);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    
    char buf[16];
    if (isnan(tempF)) {
        strcpy(buf, "--.-");
    } else {
        snprintf(buf, sizeof(buf), "%.1f", tempF);
    }
    
    tft.setFreeFont(&FreeSans24pt7b);
    tft.setCursor(20, 80);
    tft.print(buf);
    
    tft.setFreeFont(&FreeSans12pt7b);
    tft.setCursor(140, 80);
    tft.print("F");
}

void Display::drawTarget(float targetF) {
    tft.setTextColor(TFT_YELLOW, TFT_BLACK);
    tft.setFreeFont(&FreeSans18pt7b);
    tft.setCursor(20, 130);
    
    char buf[32];
    snprintf(buf, sizeof(buf), "Target: %.0f F", targetF);
    tft.print(buf);
}

void Display::drawMode(ThermoMode mode) {
    tft.setFreeFont(&FreeSans12pt7b);
    tft.setCursor(20, 170);
    
    const char* label = "UNKNOWN";
    uint16_t color = TFT_WHITE;
    
    switch (mode) {
        case MODE_OFF:     label = "OFF";     color = TFT_DARKGREY; break;
        case MODE_FAN_LO:  label = "FAN LO";  color = TFT_CYAN;     break;
        case MODE_FAN_HI: label = "FAN HI"; color = TFT_BLUE;     break;
        case MODE_COOL:    label = "COOL";    color = TFT_BLUE;     break;
        case MODE_HEAT:    label = "HEAT";    color = TFT_ORANGE;   break;
        case MODE_AUTO:    label = "AUTO";    color = TFT_GREEN;    break;
    }
    
    tft.setTextColor(color, TFT_BLACK);
    tft.print("Mode: ");
    tft.print(label);
}

void Display::drawOutputs(Thermostat& thermostat) {
    int y = 200;
    int spacing = 55;
    
    auto drawIndicator = [this, &y, spacing](const char* label, bool on, uint16_t color) {
        int x = 20;
        // LED circle
        tft.fillCircle(x + spacing / 2, y + 8, 6, on ? color : TFT_DARKGREY);
        // Label
        tft.setTextColor(on ? color : TFT_GREY, TFT_BLACK);
        tft.setFreeFont(&FreeSans9pt7b);
        tft.setCursor(x, y + 25);
        tft.print(label);
    };
    
    drawIndicator("Furnace", thermostat.isFurnaceOn(), TFT_ORANGE);
    y += spacing;
    drawIndicator("AC", thermostat.isCompressorOn(), TFT_BLUE);
    y += spacing;
    drawIndicator("Fan Hi", thermostat.isFanHiOn(), TFT_CYAN);
    y += spacing;
    drawIndicator("Fan Lo", thermostat.isFanLoOn(), TFT_GREEN);
}

void Display::drawTouchButtons() {
    // Define touch zones for UI controls
    // In a real implementation, you'd draw buttons on screen
    // For now, we set up regions for basic interaction:
    // Left side: Temperature down
    // Right side: Temperature up
    // Bottom: Mode cycling
    
    zoneCount = 0;
    
    // Temp up (top right)
    auto tempUp = [](Thermostat& t, int v) { t.incrementTarget(); };
    zones[zoneCount++] = {180, 20, 50, 50, tempUp, 0};
    
    // Temp down (top left)
    auto tempDown = [](Thermostat& t, int v) { t.decrementTarget(); };
    zones[zoneCount++] = {10, 20, 50, 50, tempDown, 0};
    
    // Mode cycle (bottom center area)
    auto cycleMode = [](Thermostat& t, int v) {
        ThermoMode next = static_cast<ThermoMode>((static_cast<int>(t.getMode()) + 1) % 6);
        t.setMode(next);
    };
    zones[zoneCount++] = {80, 250, 80, 40, cycleMode, 0};
}
