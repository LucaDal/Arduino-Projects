#include "Cube.h"
#include "SimpleOTA.h"
SimpleOTA *simpleOTA = new SimpleOTA();
Cube cube("luca","mimo");


void setup() {
  simpleOTA->begin(512, "http://lucadalessandro.hopto.org:50001", "LUCA_CUBE_06");
  simpleOTA->checkUpdates(0);//check instant the update
  cube.begin(-4471, 3157, 1281, 85, -98, 17);
}

void loop() {
  simpleOTA->checkUpdates(10);
  cube.start();
}
