#ifndef SIMLPEOTA_H
#define SIMLPEOTA_H

#include <Arduino.h>
#include "FirmwareData.h"
#include "Network.h"


//to activate serial.Print()  
//#define DEBUG 



class SimpleOTA {
private:
  FirmwareData *version;
  Network *network;
  unsigned long t1;
  const char * API_KEY;

  void initVersion();
  void initNetwork(int EEPROMSize, const char * base_url);
  void serverFirmwareCheck();
  void startDownload();
  //void updateFirmware();

public:
  SimpleOTA();
  void begin(int EEPROMSize,const char * base_url, const char * API_KEY);
  void checkUpdates(unsigned long seconds);
};

#endif