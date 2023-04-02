#include <Wire.h>

#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT);

#include "RTClib.h"
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
RTC_DS1307 rtc;

#include <ESP8266WiFi.h>
const char *ssid     = "Vodafone-A82448034";
const char *password = "2piedinidimoira";
MD5Builder md5;
void setup() {
  md5.begin()
  md5.

  // put your setup code here, to run once:
  Wire.begin(2,0);
  WiFi.begin(ssid, password);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  if(!rtc.begin()){
    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(0, 0);
    display.println("RTC error");
    display.display();
    delay(2000);
  }
  int cont = 7;
  while ((WiFi.status() != WL_CONNECTED) || cont == 0) {
    printOnScreen("Connection...",1,0,0);
    delay(500);
    cont --;
  }
  if(WiFi.status() != WL_CONNECTED){
    printOnScreen("Connected",1,0,0);
  }else{
    printOnScreen("Error WiFi",1,0,0);
  }
  //rtc.adjust(DateTime(2022, 9, 24, 18, 31, 30));
  printOnScreen("Welcome",2,0,0);
  delay(2000);
}

void printOnScreen(String text,int sizeText,int x, int y){
  display.clearDisplay();
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
    if(now.hour() < 10){
      hours = "0"+hours;
    }
    if(now.minute() < 10){
      minss = "0"+minss;
    }
  printOnScreen(hours,2,0,0);
  printOnScreen(":",2,20,0);
  printOnScreen(minss,2,28,0);

}
