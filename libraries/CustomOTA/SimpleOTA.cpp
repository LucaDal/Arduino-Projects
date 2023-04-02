#include <Arduino.h>
#include "SimpleOTA.h"

static SimpleOTA *instance = NULL;

SimpleOTA::SimpleOTA() {
  Serial.println("starting the OTA client");
  t1 = 0;
  instance = this;
}

void SimpleOTA::begin(int EEPROMSize,const char * base_url) {
  this->initNetwork(EEPROMSize, base_url);
  this->initVersion();
  this->initFileIO();
}

/**
 * @brief check update every tot seconds
 * called from the main thread
 */
void SimpleOTA::checkUpdates(unsigned long seconds) {
  
    if (millis() - t1 >= 1000 * seconds) {
      t1 = millis();
      if(network->isConnected()) {
        this->serverFirmwareCheck();
      } 
    }
}

void SimpleOTA::initVersion() {
  Serial.println("initVersion");
  version = new VersionCont(network->getFreeEEPROMAddress());
  Serial.println(version->getCurrentVersion());
}

void SimpleOTA::initNetwork(int EEPROMSize, const char * base_url) {
  Serial.println("initNetwork");
  network = new Network(base_url); 
  network->WiFiBegin(EEPROMSize);
}

void SimpleOTA::initFileIO() {
  Serial.println("initFileIO");
  fileIO = new FileIO();
  //fileIO->format();
  fileIO->listSPIFFS();
}


void SimpleOTA::serverFirmwareCheck() {
  version->setNewFirmware(network->checkVersion());
  if (version->newFirmwareVersion() == -1) {
    Serial.println("Server Not Responding");
  } else {
    if (version->hasNewUpdate()) {
      Serial.println("New Build Available!");
      Serial.println("Starting the donwload!");
      startDownload();
    }
  }
}

void SimpleOTA::startDownload() {

  //void (*ptr)(int) = &networkDownloadEvent;

  bool compareMD5Checksum = version->md5CompareTo(network->fileDownload(&fileIO, version->getFirmwareServerPath()));
  bool compareFileSize = version->fileSizeCompareTo(fileIO->getFileSize(FileIO::TEMP_BIN_FILE));

  Serial.println("======compareMD5Checksum");
  Serial.println(compareMD5Checksum);
  Serial.println("======Downloaded File SIze");
  Serial.println(compareFileSize);

  if (compareMD5Checksum && compareFileSize) {
    Serial.println("Download Complete!");
    this->updateFirmware();
  } else {
    Serial.println("Error donwloading the file!");
  }
}

void SimpleOTA::updateFirmware() {
  Serial.println("Starting the update!");
  UpdaterISPPFS *updater = new UpdaterISPPFS();
  if (updater->updateFromFS(&fileIO)) {
    Serial.println("UPDATE DONE");
    version->saveVersion(version->newFirmwareVersion());
    ESP.restart();
  } else {
    Serial.println("UPDATE FAILURE");
    delay(3000);
  }
  delete updater;
}
