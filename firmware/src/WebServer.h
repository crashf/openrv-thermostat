#pragma once
#include <Arduino.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>
#include "Thermostat.h"
#include "Sensors.h"
#include "Relays.h"

class WebServer {
public:
    WebServer();
    void begin();
    void handle();
    void updateState(Thermostat& thermostat, Sensors& sensors, Relays& relays);

private:
    AsyncWebServer server{WEB_SERVER_PORT};
    AsyncWebSocket ws{"/ws"};
    
    Thermostat* thermoRef = nullptr;
    Sensors* sensorsRef = nullptr;
    Relays* relaysRef = nullptr;
    
    void setupRoutes();
    void onWsEvent(AsyncWebSocket* server, AsyncWebSocketClient* client, 
                   AwsEventType type, void* arg, uint8_t* data, size_t len);
    void broadcastState();
};
