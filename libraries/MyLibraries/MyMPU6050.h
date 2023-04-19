#ifndef MYMPU6050_H
#define MYMPU6050_H

#include "Wire.h"
#include "I2Cdev.h"
#include "MPU6050_6Axis_MotionApps20.h"

class MyMPU6050{
    private:
        volatile bool mpuInterrupt = false;
        uint8_t interruptPin;
    public:
        MyMPU6050(uint8_t interruptPin);
        void begin(int XA, int YA, int ZA, int XG, int YG, int ZG);
        void checkInterrupt(float *Y, float *P, float *R);
};
#endif