#include "SimpleOTA.h"
SimpleOTA * simpleOTA  = new SimpleOTA();

void setup() {
  // put your setup code here, to run once:
  //simpleOTA->begin(512,"http://192.168.1.250:50001","THIS_IS_MY_OWN_API_KEY");
  Serial.begin(115200);
  simpleOTA->begin(512,"http://lucadalessandro.hopto.org:50001","THIS_IS_MY_OWN_API_KEY");
  simpleOTA->checkUpdates(10);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("bene");
  simpleOTA->checkUpdates(10);
  delay(5000);
}
