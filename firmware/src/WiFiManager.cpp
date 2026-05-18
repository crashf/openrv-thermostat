#include "WiFiManager.h"

WiFiManager::WiFiManager() {}

void WiFiManager::begin() {
    WiFi.setHostname(HOSTNAME);
    startSTA();
}

void WiFiManager::maintain() {
    if (state == STA_CONNECTING || state == STA_CONNECTED) {
        if (WiFi.status() != WL_CONNECTED) {
            unsigned long now = millis();
            if (now - lastReconnectAttempt >= RECONNECT_INTERVAL) {
                lastReconnectAttempt = now;
                Serial.println("[WiFi] Connection lost, reconnecting...");
                WiFi.reconnect();
            }
        } else if (state == STA_CONNECTING) {
            state = STA_CONNECTED;
            Serial.printf("[WiFi] Connected to %s, IP: %s\n", 
                WiFi.SSID().c_str(), WiFi.localIP().toString().c_str());
            
            // Start mDNS
            if (MDNS.begin(HOSTNAME)) {
                Serial.printf("[WiFi] mDNS responder started: %s.local\n", HOSTNAME);
            }
        }
    }
}

String WiFiManager::getIP() const {
    if (isConnected()) {
        return WiFi.localIP().toString();
    } else if (state == AP_MODE) {
        return WiFi.softAPIP().toString();
    }
    return "0.0.0.0";
}

void WiFiManager::startSTA() {
    Serial.printf("[WiFi] Connecting to %s...\n", WIFI_STA_SSID);
    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_STA_SSID, WIFI_STA_PASSWORD);
    state = STA_CONNECTING;
    lastReconnectAttempt = millis();
}

void WiFiManager::startAP() {
    Serial.println("[WiFi] Starting AP mode for setup...");
    WiFi.mode(WIFI_AP);
    WiFi.softAP(WIFI_AP_SSID, WIFI_AP_PASSWORD);
    state = AP_MODE;
    Serial.printf("[WiFi] AP mode: %s, IP: %s\n", WIFI_AP_SSID, WiFi.softAPIP().toString().c_str());
}
