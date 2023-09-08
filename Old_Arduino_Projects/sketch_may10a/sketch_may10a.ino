#include <SoftwareSerial.h>

SoftwareSerial Esp_serial(1, 2); // RX, TX

void setup(){

Esp_serial.begin(115200);

Serial.begin(115200); //Impostazioni della porta per il serial monitor

Serial.println();

Serial.println("PROVA ESP IMPOSTATO A 115200 BAUD");

Serial.println("Srivere AT (e invio) deve rispondere con OK");

Serial.println("Se non risponde premere X (e invio)");

Serial.println();

}

void loop() {

if (Esp_serial.available()) {

while (Esp_serial.available())  {

char c = Esp_serial.read();

Serial.write((char)c);

}

}

if (Serial.available()) {

delay(10);

char c = Serial.read();

if (c == 'X') {

Esp_serial.begin(9600);

Serial.println();

Serial.println("PROVA ESP IMPOSTATO A 9600 BAUD");

Serial.println("Srivere AT (e invio) deve rispondere con OK");

Serial.println();

c = Serial.read(); c = Serial.read();

}

else {

Esp_serial.print((char)c);

}

}

}
