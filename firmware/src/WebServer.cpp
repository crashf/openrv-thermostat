#include "WebServer.h"
#include <SPIFFS.h>

WebServer::WebServer() {}

void WebServer::begin() {
    if (!SPIFFS.begin(true)) {
        Serial.println("[WebServer] SPIFFS mount failed, web UI not available");
    }
    
    setupRoutes();
    server.begin();
    Serial.printf("[WebServer] HTTP server started on port %d\n", WEB_SERVER_PORT);
}

void WebServer::handle() {
    ws.cleanupClients();
}

void WebServer::updateState(Thermostat& thermostat, Sensors& sensors, Relays& relays) {
    thermoRef = &thermostat;
    sensorsRef = &sensors;
    relaysRef = &relays;
    broadcastState();
}

void WebServer::broadcastState() {
    if (!thermoRef || !sensorsRef || !relaysRef) return;
    
    StaticJsonDocument<512> doc;
    doc["currentTemp"] = sensorsRef->getTemperatureF();
    doc["targetTemp"] = thermoRef->getTargetTemp();
    doc["mode"] = thermoRef->modeString();
    doc["fanMode"] = thermoRef->getFanMode();
    doc["furnace"] = relaysRef->getFurnace();
    doc["compressor"] = relaysRef->getCompressor();
    doc["fanHi"] = relaysRef->getFanHi();
    doc["fanLo"] = relaysRef->getFanLo();
    doc["sensorValid"] = sensorsRef->isValid();
    
    String json;
    serializeJson(doc, json);
    ws.textAll(json);
}

void WebServer::setupRoutes() {
    // Static web UI files
    server.serveStatic("/", SPIFFS, "/www/").setDefaultFile("index.html");
    
    // --- REST API ---
    
    // GET /api/status — current state
    server.on("/api/status", HTTP_GET, [this](AsyncWebServerRequest* request) {
        StaticJsonDocument<512> doc;
        if (thermoRef && sensorsRef && relaysRef) {
            doc["currentTemp"] = sensorsRef->getTemperatureF();
            doc["targetTemp"] = thermoRef->getTargetTemp();
            doc["mode"] = static_cast<int>(thermoRef->getMode());
            doc["fanMode"] = static_cast<int>(thermoRef->getFanMode());
            doc["furnace"] = relaysRef->getFurnace();
            doc["compressor"] = relaysRef->getCompressor();
            doc["fanHi"] = relaysRef->getFanHi();
            doc["fanLo"] = relaysRef->getFanLo();
            doc["sensorValid"] = sensorsRef->isValid();
        }
        String json;
        serializeJson(doc, json);
        request->send(200, "application/json", json);
    });
    
    // POST /api/mode — set mode
    server.on("/api/mode", HTTP_POST, [this](AsyncWebServerRequest* request) {},
        nullptr, [this](AsyncWebServerRequest* request, uint8_t* data, size_t len, size_t index, size_t total) {
        StaticJsonDocument<128> doc;
        deserializeJson(doc, data, len);
        int mode = doc["mode"];
        if (thermoRef) {
            thermoRef->setMode(static_cast<ThermoMode>(mode));
            broadcastState();
        }
        request->send(200, "application/json", "{\"status\":\"ok\"}");
    });
    
    // POST /api/target — set target temp
    server.on("/api/target", HTTP_POST, [this](AsyncWebServerRequest* request) {},
        nullptr, [this](AsyncWebServerRequest* request, uint8_t* data, size_t len, size_t index, size_t total) {
        StaticJsonDocument<128> doc;
        deserializeJson(doc, data, len);
        float temp = doc["target"];
        if (thermoRef) {
            thermoRef->setTargetTemp(temp);
            broadcastState();
        }
        request->send(200, "application/json", "{\"status\":\"ok\"}");
    });
    
    // WebSocket for real-time updates
    ws.onEvent([this](AsyncWebSocket* server, AsyncWebSocketClient* client, 
                      AwsEventType type, void* arg, uint8_t* data, size_t len) {
        onWsEvent(server, client, type, arg, data, len);
    });
    server.addHandler(&ws);
    
    // 404 handler
    server.onNotFound([](AsyncWebServerRequest* request) {
        request->send(404, "application/json", "{\"error\":\"Not found\"}");
    });
}

void WebServer::onWsEvent(AsyncWebSocket* server, AsyncWebSocketClient* client, 
                          AwsEventType type, void* arg, uint8_t* data, size_t len) {
    if (type == WS_EVT_CONNECT) {
        Serial.printf("[WebServer] WS client #%u connected\n", client->id());
        if (thermoRef) broadcastState();
    } else if (type == WS_EVT_DISCONNECT) {
        Serial.printf("[WebServer] WS client #%u disconnected\n", client->id());
    } else if (type == WS_EVT_DATA) {
        // Handle incoming commands from web UI
        AwsFrameInfo* info = (AwsFrameInfo*)arg;
        if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
            StaticJsonDocument<256> doc;
            deserializeJson(doc, data, len);
            
            const char* cmd = doc["cmd"];
            if (strcmp(cmd, "mode") == 0 && thermoRef) {
                thermoRef->setMode(static_cast<ThermoMode>(doc["value"]));
            } else if (strcmp(cmd, "target") == 0 && thermoRef) {
                thermoRef->setTargetTemp(doc["value"]);
            } else if (strcmp(cmd, "fan") == 0 && thermoRef) {
                thermoRef->setFanMode(static_cast<FanMode>(doc["value"]));
            }
            broadcastState();
        }
    }
}
