#include "CaptivePortalHandler.h"

CaptivePortalHandler::CaptivePortalHandler() {}
CaptivePortalHandler::~CaptivePortalHandler() {}

bool CaptivePortalHandler::canHandle(AsyncWebServerRequest *request) {
  return true; // Handle all requests
}

void CaptivePortalHandler::handleRequest(AsyncWebServerRequest *request) {
  request->send(200, "text/html", "<html><body><h1>Connect to the serial monitor <a href=\"http://serialmonitor.local\">here</a>.</h1></body></html>");
}
