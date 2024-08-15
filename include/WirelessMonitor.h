#ifndef WIRELESS_MONITOR_H
#define WIRELESS_MONITOR_H

#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <WebSocketsServer.h>
#include <DNSServer.h>
#include <ESPmDNS.h>

// // Constants
// extern const char* ssid;
// extern const char* password;
// extern const byte DNS_PORT;




// WirelessMonitor Class
class WirelessMonitor {
public:
    WirelessMonitor();
    void print(const String &message);
    void setup();
    void loop();

private:
    // WebSocketsServer &webSocket;
    // String &logBuffer;
    DNSServer dnsServer;
    AsyncWebServer server;
    WebSocketsServer webSocket;
    String logBuffer;
    void onWebSocketEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t length);
    void initWiFi();
    void setupServer();
    void initDNS();
    void initmDNS();
};

// CaptivePortalHandler Class
class CaptivePortalHandler : public AsyncWebHandler {
public:
    CaptivePortalHandler();
    virtual ~CaptivePortalHandler();

    bool canHandle(AsyncWebServerRequest *request);
    void handleRequest(AsyncWebServerRequest *request);
};

#endif // WIRELESS_MONITOR_H