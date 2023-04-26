#include "MyMPU6050.h"
MyMPU6050 mympu(0);
void setup() {
  Serial.begin(115200);
  Wire.begin();
  Wire.setClock(400000);
  mympu.begin(-4471, 3157, 1281, 85, -98, 17);
}

char getCubeFace(float *Y, float *P, float *R) {
  char lastFace = 'A';
  if ((*R > -40 && *R < 40) && (*P < 40 && *P > -40)) {
    lastFace = 'A';
  } else if ((*R > 150 || *R < -150) && (*P < -140 || *P > 150)) {
    lastFace = 'F';
  } else if ((*P < -50 && *P >= -130) && ((*Y >= 40 && *Y <= 130) || *Y <= -60 || *Y >= 130 || (*Y <= 40 && *Y >= -60))) {
    lastFace = 'E';
  } else if ((*P >= 50 && *P <= 130) && ((*Y >= 30 && *Y <= 150) || (*P >= 50 && *P <= 130))) {
    lastFace = 'C';
  } else if ((*R >= -130 && *R <= -50) && ((*Y >= -70 && *Y <= 60) || (*Y >= 60 || *Y <= -140 || *Y <= 70))) {
    lastFace = 'D';
  } else if ((*R >= 50 && *R <= 120) && ((*Y >= -30 && *Y <= 150) || (*Y > 150 || *Y >= -150 || *Y <= -150))) {
    lastFace = 'B';
  } else {
    lastFace = 'N';
  }
  return lastFace;
}

void loop() {
  float Y,P,R;
  mympu.checkInterrupt(&Y,&P,&R);
    static long QTimer = millis();
  if ((long)( millis() - QTimer ) >= 1000) {
    QTimer = millis();
    Serial.print(F("\t Yaw")); Serial.print(Y);
    Serial.print(F("\t Pitch ")); Serial.print(P);
    Serial.print(F("\t Roll ")); Serial.print(R);
    Serial.print(F("\t Face: ")); Serial.print(getCubeFace(&Y,&P,&R));
    Serial.println(); 
  }
}
