#include "Cube.h"
#include "SimpleOTA.h"
SimpleOTA *simpleOTA = new SimpleOTA();
Cube cube("mimo","luca");

void setup() {
  simpleOTA->begin(512, "http://lucadalessandro.hopto.org:50001", "MIMO_CUBE_08");
  cube.begin(20,	-186,	1153,	48,	-10,	4);
}

void loop() {
  simpleOTA->checkUpdates(10);
  cube.start();
}
