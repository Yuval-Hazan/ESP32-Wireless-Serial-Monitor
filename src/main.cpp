#include <Arduino.h>
#include <WiFi.h> 
#include <AsyncTCP.h> // Asynchronous TCP library for ESP32
#include <ESPAsyncWebServer.h> // Asynchronous Web Server for ESP32
#include <WebSocketsServer.h> // WebSockets library for ESP32
#include <DNSServer.h> // DNS server for ESP32 used for captive portal
#include <ESPmDNS.h> // mDNS library for ESP32 used for resolving the hostname to an IP address
#include <SPIFFS.h> // SPIFFS library for ESP32 used to store static files for the web server

// Constants
const char* ssid = "ESP32-AP";
const char* password = "12345678";
const byte DNS_PORT = 53; // 53 is the default DNS port 

// Global variables
DNSServer dnsServer; // instance of the DNS server to handle dns requests
AsyncWebServer server(80); // instance of the web server to handle http requests
WebSocketsServer webSocket(81); // instance of the webSocket server to handle webSocket requests
String logBuffer = ""; // Global string to store logs


/**
 * @brief Handles WebSocket events.
 * 
 * This function is called when a WebSocket event occurs. It receives the WebSocket number, the event type, the payload, and the payload length as parameters.
 * If the event type is WStype_TEXT, it prints the received payload to the Serial monitor.
 * If the event type is WStype_CONNECTED, it sends the stored logs to the newly connected client.
 * 
 * @param num The WebSocket number.
 * @param type The event type.
 * @param payload The payload data.
 * @param length The length of the payload data.
 */
void onWebSocketEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t length) {
  if (type == WStype_TEXT) {
    Serial.printf("WebSocket[%u] received: %s\n", num, payload);
  } else if (type == WStype_CONNECTED) {
    // Send stored logs to the newly connected client
    webSocket.sendTXT(num, logBuffer);
  }
}

class CaptivePortalHandler : public AsyncWebHandler {
public:
  CaptivePortalHandler() {}
  virtual ~CaptivePortalHandler() {}

  bool canHandle(AsyncWebServerRequest *request){
    return true; // Handle all requests
  }

  void handleRequest(AsyncWebServerRequest *request) {
    request->send(200, "text/html", "<html><body><h1>Connect to the serial monitor <a href=\"http://serialmonitor.local\">here</a>.</h1></body></html>");
  }
};


void initSPIFFS() {
  if (!SPIFFS.begin(true)) {
    Serial.println("An error has occurred while mounting SPIFFS");
    return;
  }
}
void initWiFi() {
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password);
}

void setupServer() {
  server.serveStatic("/", SPIFFS, "/").setDefaultFile("index.html");

  server.addHandler(new CaptivePortalHandler()).setFilter(ON_AP_FILTER);

  server.onNotFound([&](AsyncWebServerRequest *request){
    request->send(200, "text/html", "<html><body><h1>Connect to the serial monitor <a href=\"http://serialmonitor.local\">here</a>.</h1></body></html>");
  });
}
void initDNS() {
  dnsServer.setErrorReplyCode(DNSReplyCode::NoError);
  dnsServer.setTTL(300);
  dnsServer.start(DNS_PORT, "*", WiFi.softAPIP());
}

void initmDNS() {
  if (!MDNS.begin("serialmonitor")) {
    Serial.println("Error setting up MDNS responder!");
  }
} 

void setup() {
  Serial.begin(9600);
  initSPIFFS();
  initWiFi();
  setupServer();
  initDNS();
  server.begin();
  Serial.println("Server started");
  initmDNS();
  Serial.println("mDNS responder started");
  webSocket.begin();
  webSocket.onEvent(onWebSocketEvent);
}

void loop() {
  dnsServer.processNextRequest();
  webSocket.loop();

  // Send changing data to the web page every second
  static unsigned long lastTime = 0;
  if (millis() - lastTime > 1000) {
    lastTime = millis();
    String message = "Current time: " + String(millis() / 1000);
    Serial.println(message); // Print to Serial for debugging
    logBuffer += message + "<br>"; // Append the message to the log buffer
    webSocket.broadcastTXT(message);
  }
}
