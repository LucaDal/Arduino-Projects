#ifndef FIRMWAREDATA_H
#define FIRMWAREDATA_H

//#define DEBUG

#include <EEPROM.h>   
#include "MyFirmware.h"
class FirmwareData {
private:
  Firmware newFirmware;
  String FIRMWARE_VERSION;
  void loadVersion();
  bool hasNewFirmware;
  int EEPROMAddress;

public:
  FirmwareData(int EEPROMAddress);
  void saveVersion(String version);
  void setNewFirmware(Firmware new_firmware_version);
  bool hasNewUpdate();
  String getFirmwareMD5Image();
  String getNewFirmwareVersion();
};

#endif 