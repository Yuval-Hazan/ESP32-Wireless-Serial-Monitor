#ifndef WIRELESS_MONITOR_H
#define WIRELESS_MONITOR_H


#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <WebSocketsServer.h>
#include <DNSServer.h>
#include <ESPmDNS.h>
#include <SPIFFS.h>

// Constants
extern const char* ssid;
extern const char* password;
extern const byte DNS_PORT;

// Global Variables
extern DNSServer dnsServer;
extern AsyncWebServer server;
extern WebSocketsServer webSocket;
extern String logBuffer;

// Function Declarations
void onWebSocketEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t length);
void initSPIFFS();
void initWiFi();
void setupServer();
void initDNS();
void initmDNS();


void setup();
void loop();

#endif // WIRELESS_MONITOR_H
