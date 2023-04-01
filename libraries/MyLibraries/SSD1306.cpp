#include "SSD1306.h"

#define SCREEN_WIDTH 128  // OLED display width, in pixels
#define SCREEN_HEIGHT 32 
Adafruit_SSD1306 display(SCREEN_WIDTH,SCREEN_HEIGHT);

void SSD1306::initialize(void){
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
}

/* Print formatted text on oled panel */
void SSD1306::print(String text, int sizeText, int x, int y) {
  display.clearDisplay();
  display.setTextSize(sizeText);
  display.setTextColor(WHITE);
  display.setCursor(x, y);
  display.println(text);
  display.display();
}
/* Print text on oled already formattes*/
void SSD1306::print(String text) {
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println(text);
  display.display();
}
/* delete everything from display */
void SSD1306::clear(void){
    display.clearDisplay();
    display.display();
}