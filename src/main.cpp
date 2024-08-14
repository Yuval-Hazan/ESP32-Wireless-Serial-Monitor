#include <WirelessMonitor.h>
ServerManager sm("ESP32-AP", "12345678");
WirelessMonitor wm(webSocket, logBuffer);

void setup() {
    sm.setup();
  // put your setup code here, to run once:
}

void loop() {
    sm.loop();
    wm.print("Hello, World!");
  // put your main code here, to run repeatedly:
}