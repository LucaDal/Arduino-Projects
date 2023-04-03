#ifndef SIMLPEOTA_H
#define SIMLPEOTA_H
#include <Arduino.h>
#include "FirmwareData.h"
#include "Network.h"

class SimpleOTA {
private:
  FirmwareData *version;
  Network *network;

  unsigned long t1;

  void initVersion();
  void initNetwork(int EEPROMSize, const char * base_url);
  void serverFirmwareCheck();
  void startDownload();
  //void updateFirmware();

public:
  SimpleOTA();
  void begin(int EEPROMSize,const char * base_url);
  void checkUpdates(unsigned long seconds);
};

#endif