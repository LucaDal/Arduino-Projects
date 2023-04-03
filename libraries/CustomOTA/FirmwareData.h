#ifndef FIRMWAREDATA_H
#define FIRMWAREDATA_H
#include <EEPROM.h>   
#include "MyFirmware.h"
class FirmwareData {
private:
  String FIRMWARE_VERSION;
  void loadVersion();
  Firmware newFirmware;
  bool hasNewFirmware;
  int EEPROMAddress;

public:
  FirmwareData(int EEPROMAddress);
  String getCurrentVersion();
  void saveVersion(String buildNum);
  void setNewFirmware(Firmware firmware);
  bool hasNewUpdate();
  bool md5CompareTo(String checksum);
  bool fileSizeCompareTo(int fileSize);
  String newFirmwareVersion();
  String getCPName();
  String getFirmwareServerPath();
};

#endif 