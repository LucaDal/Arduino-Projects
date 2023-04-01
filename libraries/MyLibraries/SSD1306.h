
#ifndef SSD1306_H
#define SSD1306_H

#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "Arduino.h"
/*Class for screen 128 by 32*/
class SSD1306{
    private:
        int SCREEN_WIDTH;
        int SCREEN_HEIGHT;
    public:
        /*Construct*/
        void initialize(void);
        /*Print formatted text to oled*/
        void print(String text, int sizeText, int x, int y);
        /*Print text to oled already formatted*/
        void print(String text);
        /*Delete everything from the oled*/
        void clear(void);
};


#endif