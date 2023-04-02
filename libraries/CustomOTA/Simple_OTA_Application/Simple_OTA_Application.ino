#include "SimpleOTA.h"
SimpleOTA *simpleOTA;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("Starting the ota");
  simpleOTA = new SimpleOTA();
  simpleOTA->begin(512,"http://192.168.1.12:9001/api");
  
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("funziono");
  simpleOTA->checkUpdates(10);
  delay(5000);
}
