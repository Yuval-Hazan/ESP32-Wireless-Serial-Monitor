#ifndef CAPTIVE_PORTAL_HANDLER_H
#define CAPTIVE_PORTAL_HANDLER_H

#include <ESPAsyncWebServer.h>

class CaptivePortalHandler : public AsyncWebHandler {
public:
  CaptivePortalHandler();
  virtual ~CaptivePortalHandler();

  bool canHandle(AsyncWebServerRequest *request);
  void handleRequest(AsyncWebServerRequest *request);
};

#endif // CAPTIVE_PORTAL_HANDLER_H
