#include "SSD1306.h"

SSD1306::SSD1306(int SCREEN_WIDTH,int SCREEN_HEIGHT){
  display = new Adafruit_SSD1306(SCREEN_WIDTH,SCREEN_HEIGHT);
}

//Adafruit_SSD1306 display(128,32);

void SSD1306::begin(void){
  display->begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display->setTextColor(WHITE);
}

/* Print formatted text on oled panel */
void SSD1306::print(String text, int sizeText, int x, int y) {
  display->clearDisplay();
  display->setTextSize(sizeText);
  display->setCursor(x, y);
  display->println(text);
  display->display();
}
/* Print text on oled already formattes*/
void SSD1306::print(String text) {
  display->clearDisplay();
  display->setTextSize(2);
  display->setCursor(0, 0);
  display->println(text);
  display->display();
}

/*It writes content till clear is setted as False, use clear = True at the beginning*/
void SSD1306::printf(String text, int sizeText, int x, int y, bool clear){
  if (clear){
    display->clearDisplay();
  }
  display->setTextSize(sizeText);
  display->setCursor(x, y);
  display->println(text);
  display->display();
}


/* delete everything from display */
void SSD1306::clear(void){
    display->clearDisplay();
    display->display();
}