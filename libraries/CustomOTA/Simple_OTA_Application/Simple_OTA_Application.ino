#include "SimpleOTA.h"
SimpleOTA *simpleOTA;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  simpleOTA = new SimpleOTA();
  simpleOTA->begin(512,"http://192.168.1.12:9001");
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("bene!");
  simpleOTA->checkUpdates(10);
  delay(5000);
}
