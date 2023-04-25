#ifndef CUBE_H
#define CUBE_H

#include "PCF8574.h"
#include "MyMPU6050.h"
#include <ESP8266HTTPClient.h>

#define RX 3
#define TX 1
#define TIME_DELAY_TO_RESEND_FACE 300000  //5 minuti


class Cube{
    private:
        const char * THIS_CUBE_CODE;
        const char * RELATED_CUBE_CODE;
        WiFiClient client;
        char faceToSend;
        char oldFace = ' ';
        uint8_t contFaceSimilar = 0;
        size_t contToSendData = 0;
        size_t secondToWaitBeforeSendingData = 0;  //at startup
        bool dataIsSent = false;
        bool faceIsSetted = false;
        uint8_t sendingToRelatedCube = 0;
        //==============SERVER INFO=====================
        const char *host = "lucadalessandro.hopto.org";
        const uint16_t port = 50000;

        bool updateServer();
        void sendFace();
        bool connectToTheServer();
        void checkUpdateFromMPU();
        void checkConnection();
        void checkMessage(String serverMessage);
    public:
        Cube(const char *THIS_CUBE_CODE,const char *RELATED_CUBE_CODE);
        void begin(int XA, int YA, int ZA, int XG, int YG,int ZG);
        void start();
};

#endif