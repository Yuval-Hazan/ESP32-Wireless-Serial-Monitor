#include <WirelessMonitor.h>
WirelessMonitor wm(webSocket, logBuffer);

void setup() {
    wm.setup();
  // put your setup code here, to run once:
}

void loop() {
    wm.loop();
    wm.print("Hello, World!");
  // put your main code here, to run repeatedly:
}