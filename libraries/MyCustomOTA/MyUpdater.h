#ifndef MYUPDATER_H
#define MYUPDATER_H

//#define DEBUG

#include "MyUpdater.h"
#include <ESP8266httpUpdate.h>

class MyUpdater{
    public:
        MyUpdater(String md5Checksum);
        bool startUpdate(WiFiClient wifiClient, String BASE_URL);
};

#endif