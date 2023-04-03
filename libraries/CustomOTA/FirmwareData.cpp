#include "FirmwareData.h"
FirmwareData::FirmwareData(int EEPROMAddress) {
  this->EEPROMAddress = EEPROMAddress;
  this->loadVersion();
  hasNewFirmware = false;
}

String FirmwareData::getCurrentVersion() {
  return FIRMWARE_VERSION;
}

void FirmwareData::loadVersion() {
  //EEProm Already begun in network (myWifiManager)
  uint8_t firstValue = EEPROM.read(EEPROMAddress);
  uint8_t SecondValue = EEPROM.read(EEPROMAddress+1);
  this-> FIRMWARE_VERSION = String(firstValue) + '.' + String(SecondValue); ;
}

String getValue(String data, char separator, int index) {
  int found = 0;
  int strIndex[] = { 0, -1 };
  int maxIndex = data.length() - 1;

  for (int i = 0; i <= maxIndex && found <= index; i++) {
    if (data.charAt(i) == separator || i == maxIndex) {
      found++;
      strIndex[0] = strIndex[1] + 1;
      strIndex[1] = (i == maxIndex) ? i + 1 : i;
    }
  }
  return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}

void FirmwareData::saveVersion(String buildNum) {
  uint8_t firstValue =  getValue(buildNum,'.',0).toInt();
  uint8_t secondValue = getValue(buildNum,'.',1).toInt();
   Serial.printf("firstValue = %i, secondValue = %i\n",firstValue,secondValue);
  EEPROM.write(EEPROMAddress, firstValue);
  EEPROM.write(EEPROMAddress+1, secondValue);
  EEPROM.commit();
}

void FirmwareData::setNewFirmware(Firmware firmware) {

  if (FIRMWARE_VERSION == firmware.build_num) {
      return;
  }
  newFirmware = firmware;
  if (newFirmware.build_num != "-1") {
    Serial.printf("buildNum: %s\n", newFirmware.build_num.c_str());
    Serial.printf("fileSize: %d\n", newFirmware.file_size);
    Serial.printf("MD5Checksum: %s\n", newFirmware.md5_checksum.c_str());
    Serial.printf("build_date: %s\n", newFirmware.build_date.c_str());
    Serial.printf("company: %s\n", newFirmware.company.c_str());
    Serial.printf("server_file_path: %s\n", newFirmware.server_file_path.c_str());
  }
  hasNewFirmware = true;
}

bool FirmwareData::hasNewUpdate() {
  return hasNewFirmware;
}

bool FirmwareData::md5CompareTo(String checksum) {
  return newFirmware.md5_checksum == checksum;
}

bool FirmwareData::fileSizeCompareTo(int fileSize) {
  return newFirmware.file_size == fileSize;
}

String FirmwareData::newFirmwareVersion() {
  return newFirmware.build_num;
}

String FirmwareData::getCPName() {
  return newFirmware.company != NULL ? newFirmware.company : "";
}

String FirmwareData::getFirmwareServerPath() {
  return newFirmware.server_file_path != NULL ? newFirmware.server_file_path : "";
}
