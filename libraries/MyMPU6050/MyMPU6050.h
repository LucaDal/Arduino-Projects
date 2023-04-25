#ifndef MYMPU6050_H
#define MYMPU6050_H

//#define DEBUG

#include "Wire.h"
#include "I2Cdev.h"
#include "MPU6050_6Axis_MotionApps20.h"

class MyMPU6050{
    private:
        int interruptPin;
    public:
        MyMPU6050(int interruptPin);
        void begin(int XA, int YA, int ZA, int XG, int YG, int ZG);
        bool checkInterrupt(float *Y, float *P, float *R);
        void stopInterrupt();
};

#endif