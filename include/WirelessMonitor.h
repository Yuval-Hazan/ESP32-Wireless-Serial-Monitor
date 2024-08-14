#ifndef WIRELESS_MONITOR_H
#define WIRELESS_MONITOR_H

#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <WebSocketsServer.h>
#include <DNSServer.h>
#include <ESPmDNS.h>

// Constants
extern const char* ssid;
extern const char* password;
extern const byte DNS_PORT;

// // Global Variables
extern DNSServer dnsServer;
extern AsyncWebServer server;
extern WebSocketsServer webSocket;
extern String logBuffer;

// WirelessMonitor Class
class WirelessMonitor {
public:
    WirelessMonitor(WebSocketsServer &ws, String &logBufferRef);
    void print(const String &message);
    void setup();
    void loop();

private:
    WebSocketsServer &webSocket;
    String &logBuffer;
};


// CaptivePortalHandler Class
class CaptivePortalHandler : public AsyncWebHandler {
public:
    CaptivePortalHandler();
    virtual ~CaptivePortalHandler();

    bool canHandle(AsyncWebServerRequest *request);
    void handleRequest(AsyncWebServerRequest *request);
};

// ServerManager Class
class ServerManager {
public:
    ServerManager(const char* ssid, const char* password);
    void setup();
    void loop();


private:
    const char* ssid;
    const char* password;
    
    void onWebSocketEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t length);
    void initWiFi();
    void setupServer();
    void initDNS();
    void initmDNS();
};



#endif // WIRELESS_MONITOR_H