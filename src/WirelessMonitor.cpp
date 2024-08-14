// Headers
#include "WirelessMonitor.h"
#include "EmbeddedFiles.h"

// Constants
const char* ssid = "ESP32-AP";
const char* password = "12345678";
const byte DNS_PORT = 53;

// Global Variables
DNSServer dnsServer;
AsyncWebServer server(80);
WebSocketsServer webSocket(81);
String logBuffer = ""; // Global string to store logs

// WirelessMonitor Class Implementation
WirelessMonitor::WirelessMonitor(WebSocketsServer &ws, String &logBufferRef)
    : webSocket(ws), logBuffer(logBufferRef) {}
ServerManager sm("ESP32-AP", "12345678");
void WirelessMonitor::print(const String &message) {
    Serial.println(message);             // Print to Serial Monitor
    logBuffer += message + "<br>";       // Append to log buffer
    webSocket.broadcastTXT(message.c_str());     // Broadcast via WebSocket
}
void WirelessMonitor::setup(){
    sm.setup();
}
void WirelessMonitor::loop(){
    sm.loop();
}

// CaptivePortalHandler Class Implementation
CaptivePortalHandler::CaptivePortalHandler() {
    // Constructor implementation
}

CaptivePortalHandler::~CaptivePortalHandler() {
    // Destructor implementation
}

bool CaptivePortalHandler::canHandle(AsyncWebServerRequest *request) {
    // Handle all requests
    return true;
}

void CaptivePortalHandler::handleRequest(AsyncWebServerRequest *request) {
    // Serve a simple HTML page
    request->send(200, "text/html", "<html><body><h1>Connect to the serial monitor <a href=\"http://serialmonitor.local\">here</a>.</h1></body></html>");
}

// ServerManager Class Implementation
ServerManager::ServerManager(const char* ssid, const char* password)
    : ssid(ssid), password(password), wirelessMonitor(webSocket, logBuffer) {}

void ServerManager::setup() {
    Serial.begin(9600);
    initWiFi();
    setupServer();
    initDNS();
    server.begin();
    Serial.println("Server started");
    initmDNS();
    Serial.println("mDNS responder started");
    webSocket.begin();
    webSocket.onEvent([this](uint8_t num, WStype_t type, uint8_t *payload, size_t length) {
        this->onWebSocketEvent(num, type, payload, length);
    });
}

void ServerManager::loop() {
    dnsServer.processNextRequest();
    webSocket.loop();
}

void ServerManager::onWebSocketEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t length) {
    if (type == WStype_TEXT) {
        Serial.printf("WebSocket[%u] received: %s\n", num, payload);
    } else if (type == WStype_CONNECTED) {
        webSocket.sendTXT(num, logBuffer);  // Send stored logs to the newly connected client
    }
}

void ServerManager::initWiFi() {
    WiFi.mode(WIFI_AP);
    WiFi.softAP(ssid, password);
}

void ServerManager::setupServer() {
    server.on("/", HTTP_GET, serveIndexHtml);    // Serve the embedded HTML
    server.on("/style.css", HTTP_GET, serveStyleCss); // Serve the embedded CSS
    server.on("/script.js", HTTP_GET, serveScriptJs); // Serve the embedded JS

    server.addHandler(new CaptivePortalHandler()).setFilter(ON_AP_FILTER);

    server.onNotFound([&](AsyncWebServerRequest *request){
        request->send(200, "text/html", "<html><body><h1>Connect to the serial monitor <a href=\"http://serialmonitor.local\">here</a>.</h1></body></html>");
    });
}

void ServerManager::initDNS() {
    dnsServer.setErrorReplyCode(DNSReplyCode::NoError);
    dnsServer.setTTL(300);
    dnsServer.start(DNS_PORT, "*", WiFi.softAPIP());
}

void ServerManager::initmDNS() {
    if (!MDNS.begin("serialmonitor")) {
        Serial.println("Error setting up MDNS responder!");
    }
}

// void onWebSocketEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t length) {
//   if (type == WStype_TEXT) {
//     Serial.printf("WebSocket[%u] received: %s\n", num, payload);
//   } else if (type == WStype_CONNECTED) {
//     // Send stored logs to the newly connected client
//     webSocket.sendTXT(num, logBuffer);
//   }
// }

// void initWiFi() {
//   WiFi.mode(WIFI_AP);
//   WiFi.softAP(ssid, password);
// }

// void setupServer() {
//   server.on("/", HTTP_GET, serveIndexHtml);    // Serve the embedded HTML
//   server.on("/style.css", HTTP_GET, serveStyleCss); // Serve the embedded CSS
//   server.on("/script.js", HTTP_GET, serveScriptJs); // Serve the embedded JS

//   server.addHandler(new CaptivePortalHandler()).setFilter(ON_AP_FILTER);

//   server.onNotFound([&](AsyncWebServerRequest *request){
//     request->send(200, "text/html", "<html><body><h1>Connect to the serial monitor <a href=\"http://serialmonitor.local\">here</a>.</h1></body></html>");
//   });
// }

// void initDNS() {
//   dnsServer.setErrorReplyCode(DNSReplyCode::NoError);
//   dnsServer.setTTL(300);
//   dnsServer.start(DNS_PORT, "*", WiFi.softAPIP());
// }

// void initmDNS() {
//   if (!MDNS.begin("serialmonitor")) {
//     Serial.println("Error setting up MDNS responder!");
//   }
// }
// void wifiPrint(String message) {
//   Serial.println(message);
//   logBuffer += message + "<br>";
//   webSocket.broadcastTXT(message);
// }

// void setup() {
//   Serial.begin(9600);
//   initWiFi();
//   setupServer();
//   initDNS();
//   server.begin();
//   Serial.println("Server started");
//   initmDNS();
//   Serial.println("mDNS responder started");
//   webSocket.begin();
//   webSocket.onEvent(onWebSocketEvent);
// }



// void loop() {
//   dnsServer.processNextRequest();
//   webSocket.loop();
// }



