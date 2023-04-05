#include <SSD1306.h>
#include "Wire.h"
#include "SimpleOTA.h"
SimpleOTA *simpleOTA;
SSD1306 oled(128,32);

void setup() {
  Wire.begin(2, 0);
  oled.begin();
  delay(3000);
  oled.print("cerco nuovo firmware...");
  simpleOTA = new SimpleOTA();
  simpleOTA->begin(512,"http://192.168.1.6:9001","APY_TOKEN");//it includes WifiManager
}

void loop() {
  simpleOTA->checkUpdates(10);
  // oled.print("va");
  // delay(1000);
  //   oled.print("bene");
  // delay(1000);
}
