#include <Arduino.h>

char A;
char B;
char C;
char D;

void setup() {
  // put your setup code here, to run once:
  pinMode(2,OUTPUT);
  pinMode(3,OUTPUT);
  pinMode(4,OUTPUT);
  pinMode(5,OUTPUT);
  Serial.begin(9600);
}

/*
this function will sett A B C D (global value) to light the right segment
*/
void tobin(int value) {
  int i = 0;
  int mul = 10;
  for (int i = 1; i <= 4; i++) {
    i += (value%2) * mul;
    mul *= 10;
  }
  Serial.println(i);
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(1000);
  tobin(7);
}
