#pragma once
#include <Arduino.h>
#include <WiFi.h>
#include <ESPmDNS.h>
#include "config.h"

class WiFiManager {
public:
    WiFiManager();
    void begin();
    void maintain();
    bool isConnected() const { return WiFi.status() == WL_CONNECTED; }
    String getIP() const;
    
private:
    enum State { STA_CONNECTING, STA_CONNECTED, AP_MODE };
    State state = STA_CONNECTING;
    unsigned long lastReconnectAttempt = 0;
    static const unsigned long RECONNECT_INTERVAL = 30000; // 30 seconds
    
    void startSTA();
    void startAP();
};
