#include <WirelessMonitor.h>
WirelessMonitor wm;

void setup() {
  wm.setup();
  // put your setup code here, to run once:
}

void loop() {
  wm.loop();
  wm.print(
      "Hello, World!Hello, World!Hello, World!Hello, World!Hello, World!Hello, "
      "World!Hello, World!Hello, World!Hello, World!Hello, World!Hello, "
      "World!Hello, World!Hello, World!Hello, World!Hello, World!Hello, "
      "World!Hello, World!Hello, World!Hello, World!Hello, World!Hello, "
      "World!Hello, World!");
  // put your main code here, to run repeatedly:
}