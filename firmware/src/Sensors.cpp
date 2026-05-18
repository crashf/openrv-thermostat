#include "Sensors.h"

Sensors::Sensors() : oneWire(PIN_ONE_WIRE), ds18b20(&oneWire) {}

void Sensors::init() {
    Serial.println("[Sensors] Initializing DS18B20 on pin " + String(PIN_ONE_WIRE));
    ds18b20.begin();
    
    uint8_t count = ds18b20.getDeviceCount();
    Serial.printf("[Sensors] Found %d DS18B20 device(s)\n", count);
    
    if (count == 0) {
        Serial.println("[Sensors] WARNING: No DS18B20 sensors detected!");
    }
}

void Sensors::read() {
    ds18b20.requestTemperatures();
    delay(100); // Allow conversion (9-bit ~93ms, 10-bit ~187ms)
    
    tempC = ds18b20.getTempCByIndex(0);
    
    if (tempC != DEVICE_DISCONNECTED_C && tempC != 85.0) {
        tempF = (tempC * 9.0 / 5.0) + 32.0;
        valid = true;
        lastReadSuccess = millis();
    } else {
        Serial.println("[Sensors] ERROR: DS18B20 read failed!");
        valid = false;
    }
}
