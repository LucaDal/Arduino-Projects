#ifndef MYMPU6050_H
#define MYMPU6050_H

class MyMPU6050{
    private:
        void GetDMP();
        void MPUMath();
        void getYPR(float *Y, float *P, float *R);
    public:
        void MyMPU6050::begin(int *XA, int *YA, int *ZA, int *XG, int *YG, int *ZG);
        void MyMPU6050::checkInterrupt(float *Y, float *P, float *R);
};
#endif