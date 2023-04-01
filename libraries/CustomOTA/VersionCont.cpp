#include "VersionCont.h"
#include <EEPROM.h>
#define EEPROM_SIZE 512

VersionCont::VersionCont(int EEPROMAddress) {
  this->loadVersion(EEPROMAddress);
  hasNewFirmware = false;
}

int VersionCont::getCurrentVersion() {
  return firmwareVersion;
}

void VersionCont::loadVersion(int EEPROMAddress) {
  EEPROM.begin(EEPROM_SIZE);
  firmwareVersion = EEPROM.read(EEPROMAddress);

  if (firmwareVersion >= 255 || firmwareVersion == 0) {
    firmwareVersion = 1;
    this->saveVersion(firmwareVersion,EEPROMAddress);
  }
}

void VersionCont::saveVersion(int buildNum, int EEPROMAddress ) {
  if (buildNum > 255) buildNum = 255;
  EEPROM.write(EEPROMAddress, buildNum);
  EEPROM.commit();
}

void VersionCont::setNewFirmware(Firmware firmware) {
  newFirmware = firmware;

  if (newFirmware.build_num != -1) {
    Serial.printf("buildNum: %d\n", newFirmware.build_num);
    Serial.printf("fileSize: %d\n", newFirmware.file_size);
    Serial.printf("MD5Checksum: %s\n", newFirmware.md5_checksum.c_str());
    Serial.printf("build_date: %s\n", newFirmware.build_date.c_str());
    Serial.printf("company: %s\n", newFirmware.company.c_str());
    Serial.printf("server_file_path: %s\n", newFirmware.server_file_path.c_str());

    if (firmwareVersion < newFirmware.build_num) {
      hasNewFirmware = true;
    }
  }
}

bool VersionCont::hasNewUpdate() {
  return hasNewFirmware;
}

bool VersionCont::md5CompareTo(String checksum) {
  return newFirmware.md5_checksum == checksum;
}

bool VersionCont::fileSizeCompareTo(int fileSize) {
  return newFirmware.file_size == fileSize;
}

int VersionCont::newFirmwareVersion() {
  return newFirmware.build_num;
}

String VersionCont::getCPName() {
  return newFirmware.company != NULL ? newFirmware.company : "";
}

String VersionCont::getFirmwareServerPath() {
  return newFirmware.server_file_path != NULL ? newFirmware.server_file_path : "";
}
