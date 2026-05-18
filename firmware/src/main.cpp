#include <Arduino.h>
#include "config.h"
#include "Thermostat.h"
#include "Sensors.h"
#include "Relays.h"
#include "Display.h"
#include "WebServer.h"
#include "WiFiManager.h"

Thermostat thermostat;
Sensors sensors;
Relays relays;
Display display;
WebServer webServer;
WiFiManager wifi;

unsigned long lastSensorRead = 0;
unsigned long lastControlLoop = 0;

void setup() {
    Serial.begin(115200);
    Serial.println("\n========================================");
    Serial.println("  OpenRV Thermostat v0.1");
    Serial.println("========================================");

    // Initialize subsystems
    relays.init();
    sensors.init();
    display.init();
    
    // Start WiFi (STA first, fallback to AP for setup)
    wifi.begin();
    
    // Start web server
    webServer.begin();
    
    // Load saved settings
    thermostat.loadSettings();
    
    // Pass references to web server for state reporting
    webServer.updateState(thermostat, sensors, relays);
    
    Serial.println("Setup complete. Thermostat running.");
}

void loop() {
    unsigned long now = millis();

    // 1. Read temperature sensors every second
    if (now - lastSensorRead >= SENSOR_READ_INTERVAL_MS) {
        lastSensorRead = now;
        sensors.read();
        
        float tempF = sensors.getTemperatureF();
        if (!isnan(tempF)) {
            thermostat.updateTemperature(tempF);
        }
    }

    // 2. Control loop every 5 seconds
    if (now - lastControlLoop >= CONTROL_LOOP_INTERVAL_MS) {
        lastControlLoop = now;
        
        thermostat.evaluate();
        
        // Apply relay states based on thermostat output
        relays.setFurnace(thermostat.isFurnaceOn());
        relays.setCompressor(thermostat.isCompressorOn());
        relays.setFanHi(thermostat.isFanHiOn());
        relays.setFanLo(thermostat.isFanLoOn());
        
        // Update display
        display.update(thermostat, sensors);
        
        // Log state
        Serial.printf("[%.1f°F] Mode=%s Target=%.1f Outputs: F=%d C=%d FH=%d FL=%d\n",
            sensors.getTemperatureF(),
            thermostat.modeString(),
            thermostat.getTargetTemp(),
            thermostat.isFurnaceOn() ? 1 : 0,
            thermostat.isCompressorOn() ? 1 : 0,
            thermostat.isFanHiOn() ? 1 : 0,
            thermostat.isFanLoOn() ? 1 : 0
        );
    }

    // 3. Handle touch / UI
    display.handleTouch(thermostat);
    
    // 4. Handle web requests
    webServer.handle();
    
    // 5. WiFi reconnect if needed
    wifi.maintain();
    
    // Yield to WiFi stack
    delay(1);
}
