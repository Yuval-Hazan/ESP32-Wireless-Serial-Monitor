#include "WirelessMonitor.h"
#include "CaptivePortalHandler.h"

// Constants
const char* ssid = "ESP32-AP";
const char* password = "12345678";
const byte DNS_PORT = 53;

// Global Variables
DNSServer dnsServer;
AsyncWebServer server(80);
WebSocketsServer webSocket(81);
String logBuffer = ""; // Global string to store logs

void onWebSocketEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t length) {
  if (type == WStype_TEXT) {
    Serial.printf("WebSocket[%u] received: %s\n", num, payload);
  } else if (type == WStype_CONNECTED) {
    // Send stored logs to the newly connected client
    webSocket.sendTXT(num, logBuffer);
  }
}

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
