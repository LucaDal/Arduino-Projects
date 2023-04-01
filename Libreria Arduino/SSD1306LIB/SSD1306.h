
#ifndef SSD1306_H
#define SSD1306_H

#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "Arduino.h"

class SSD1306{
    private:
        int SCREEN_WIDTH;
        int SCREEN_HEIGHT;
    public:
        void initialize(int width, int height);
        void print(String text, int sizeText, int x, int y);
        void clear(void);
};


#endif