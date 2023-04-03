#ifndef MYUPDATER_H
#define MYUPDATER_H

#include "MyUpdater.h"
#include <ESP8266httpUpdate.h>

class MyUpdater{
    public:
        MyUpdater();
        bool startUpdate(WiFiClient wifiClient, String BASE_URL);
};

#endif