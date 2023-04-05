#define DEBUG
#include "SimpleOTA.h"
SimpleOTA * simpleOTA  = new SimpleOTA();

void setup() {
  // put your setup code here, to run once:
  #ifdef DEBUG
    Serial.begin(115200);
  #endif
  simpleOTA->begin(512,"http://192.168.1.4:9001","THIS_IS_MY_OWN_API_KEY");
}

void loop() {
  // put your main code here, to run repeatedly:
  #ifdef DEBUG
    Serial.println("andra?");
  #endif
  simpleOTA->checkUpdates(10);
  delay(5000);
}
