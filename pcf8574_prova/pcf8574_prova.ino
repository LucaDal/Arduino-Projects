/*
 Blink led on PIN0
 by Mischianti Renzo <http://www.mischianti.org>

 https://www.mischianti.org/2019/01/02/pcf8574-i2c-digital-i-o-expander-fast-easy-usage/
*/

#include "Arduino.h"
#include "PCF8574.h"
#include <SSD1306.h>
SSD1306 oled;
PCF8574 pcf8574(0x20,2,0);

void setup()
{
  Wire.begin(2, 0);
  oled.initialize();
  delay(1000);
  oled.print("Init pcf8574...");
	pcf8574.pinMode(P1, OUTPUT);
  pcf8574.pinMode(P6, OUTPUT);
  pcf8574.pinMode(P7, OUTPUT);

	oled.print("Init pcf8574...");
	if (pcf8574.begin()){
		oled.print("OK");
	}else{
		oled.print("KO");
	}
  delay(3000);
}

void loop()
{
  oled.print("HIGH P7 - LOW P6...");
	pcf8574.digitalWrite(P7, HIGH);
  pcf8574.digitalWrite(P6, LOW);
	delay(1000);
  oled.print("LOW P7 - HIGH P6...");
	pcf8574.digitalWrite(P7, LOW);
  pcf8574.digitalWrite(P6, HIGH);
	delay(1000);
}
