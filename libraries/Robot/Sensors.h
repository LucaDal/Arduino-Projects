#include <Sensors.h>

#define DEBUG 1

#define S_START 1
#define S_FORWARD 2
#define S_TURN_LEFT 3
#define S_TURN_RIGHT 4
#define SPEED_STD 20
#define SPEED_RED 15

char colPrint; //Aggiunta per LCD
int status = S_START;
Motors *motors;
Sensors *sensors;
char colL, colC, colR;

void setup() {
  Serial.begin(9600);
  Serial.println("Avvio del robot...");
  //Wire.begin();
  motors = new Motors();
  sensors = new Sensors();
}

void loop() {
  Serial.println(status);
  switch (status) {
    colL = sensors->getColorLeft();
    colR = sensors->getColorRight();
  case S_START:
    colC = sensors->getColorCenter();
    if(colL == COL_WHITE && colC == COL_BLACK && colR == COL_WHITE) {  // Vai dritto
      status = S_FORWARD;
      motors->travel(SPEED_STD);
      Serial.println(status);
    }
    break;
  case S_FORWARD:
    if(colL == COL_WHITE && colR == COL_BLACK) {  // Inizia curva a destra
      status = S_TURN_RIGHT;
      motors->spArc(SPEED_RED, -1);
    } else if(colL == COL_BLACK && colR == COL_WHITE) { // Inizia curva a sinistra
      status = S_TURN_LEFT;
      motors->spArc(SPEED_RED, 1);
    }
    Serial.println(status);
    break;
  case S_TURN_RIGHT:
    if(colL == COL_WHITE && colR == COL_BLACK) {  // Inizia ad andare dritto
      status = S_FORWARD;
      motors->travel(SPEED_STD);
    }
    Serial.println(status);
    break;
  case S_TURN_LEFT:
    if(colL == COL_WHITE && colR == COL_BLACK) {  // Inizia ad andare dritto
      status = S_FORWARD;
      motors->travel(SPEED_STD);
    }
    Serial.println(status);
    break;


}


}

/*  if(status == S_START) {
    colC = sensors->getColorCenter();
    if(colL == COL_WHITE && colC == COL_BLACK && colR == COL_WHITE) { // Vai dritto
      status = S_FORWARD;
      motors->travel(SPEED_STD);
    }
  } else if(status == S_FORWARD) {  // Sta andando dritto
    if(colL == COL_WHITE && colR == COL_BLACK) {  // Inizia curva a destra
      status = S_TURN_RIGHT;
      motors->spArc(SPEED_RED, -1);
    } else if(colL == COL_BLACK && colR == COL_WHITE) { // Inizia curva a sinistra
      status = S_TURN_LEFT;
      motors->spArc(SPEED_RED, 1);
    }
  } else if(status == S_TURN_RIGHT) { // Sta curvando a destra
    if(colR == COL_WHITE) {  // Inizia ad andare dritto
      status = S_FORWARD;
      motors->travel(SPEED_STD);
    }
  } else if(status == S_TURN_LEFT) {  // Sta curvando a sinistra
    if(colL == COL_WHITE) {  // Inizia ad andare dritto
      status = S_FORWARD;
      motors->travel(SPEED_STD);
    }
  }*/
