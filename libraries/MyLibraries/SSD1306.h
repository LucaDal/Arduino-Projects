
#ifndef SSD1306_H
#define SSD1306_H

#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "Arduino.h"

class SSD1306{
    private:
        Adafruit_SSD1306 *display;
    public:
        SSD1306(int SCREEN_WIDTH,int SCREEN_HEIGHT);
        void begin(void);
        /*Print formatted text to oled*/
        void print(String text, int sizeText, int x, int y);
        /*Print text to oled already formatted*/
        void print(String text);
        /*Delete everything from the oled*/
        void clear(void);
};


#endif