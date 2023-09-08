// Date and time functions using a DS1307 RTC connected via I2C and Wire lib
#include "RTClib.h"
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define SCREEN_WIDTH 128  // OLED display width, in pixels
#define SCREEN_HEIGHT 32  // OLED display height, in pixels
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT);

#include <Wire.h>

RTC_DS1307 rtc;
char daysOfTheWeek[7][12] = { "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday" };

void setup() {
  Wire.begin(2, 0);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  if (!rtc.begin()) {
    printOnScreen("error rtc", 1, 0, 0);
    while (1) delay(100);
  }

  if (!rtc.isrunning()) {
    printOnScreen("error rtc", 1, 0, 0);
  }
  rtc.adjust(DateTime(2022, 12, 16, 17, 39, 0));
}

void printOnScreen(String text, int sizeText, int x, int y) {
  display.setTextSize(sizeText);
  display.setTextColor(WHITE);
  display.setCursor(x, y);
  display.println(text);
  display.display();
}

void loop() {
  DateTime now = rtc.now();
  String hours = String(now.hour());
  String minss = String(now.minute());
  printOnScreen(hours, 2, 0, 0);
  printOnScreen(":", 2, 25, 0);
  printOnScreen(minss, 2, 55, 0);
  display.clearDisplay();
  delay(1000);
}
