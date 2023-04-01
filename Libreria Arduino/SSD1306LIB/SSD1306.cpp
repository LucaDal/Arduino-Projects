#include "SSD1306.h"
Adafruit_SSD1306 display(width, height);
void SSD1306::initialize(int width, int height){
    int SCREEN_WIDTH = width;
    int SCREEN_HEIGHT = height;
}
/* stampa sullo schermo oled testo */
void SSD1306::print(String text, int sizeText, int x, int y) {
  display.clearDisplay();
  display.setTextSize(sizeText);
  display.setTextColor(WHITE);
  display.setCursor(x, y);
  display.println(text);
  display.display();
}
/* delete everything from display */
void SSD1306::clear(void){
    display.clearDisplay();
}