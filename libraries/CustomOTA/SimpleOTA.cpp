#include <Arduino.h>
#include "SimpleOTA.h"

static SimpleOTA *instance = NULL;

SimpleOTA::SimpleOTA() {
  t1 = 0;
  t2 = 0;
  currentState = NONE;
  instance = this;
}

SimpleOTA::~SimpleOTA() {
}


void networkDownloadEvent(int percent) {
  Serial.print("networkDownloadEvent downloadPercent: ");
  if (percent == 0) {
    instance->currentState = FIRMWARE_DOWNLOAD_START;
  }
  Serial.println(percent);
}

void SimpleOTA::begin() {
  this->initVersion();
  this->initNetwork();
  this->initFileIO();
}

/**
 * @brief check update every tot seconds
 */
void SimpleOTA::checkUpdates(long seconds) {
  if(network->isConnected()) {
    if (millis() - t1 >= 1000 * seconds) {
      t1 = millis();
      this->serverFirmwareCheck();
    } 
  }
}

void SimpleOTA::initVersion() {
  Serial.println("initVersion");
  version = new VersionCont(network->getFreeEEPROMAddress());
  Serial.println(version->getCurrentVersion());
}

void SimpleOTA::initNetwork() {
  Serial.println("initNetwork");
  network = new Network();
  currentState = NETWORK_BEGIN;
  network->WiFiBegin();
}

void SimpleOTA::initFileIO() {
  Serial.println("initFileIO");
  fileIO = new FileIO();
  fileIO->format();
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

  void (*ptr)(int) = &networkDownloadEvent;

  bool compareMD5Checksum = version->md5CompareTo(network->fileDownload(ptr, &fileIO, version->getFirmwareServerPath()));
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
    currentState = SERVER_FOUND;
  }
}

void SimpleOTA::updateFirmware() {
  currentState = FIRMWARE_DOWNLOAD_START;
  Serial.println("Starting the update!");
  UpdaterISPPFS *updater = new UpdaterISPPFS();
  if (updater->updateFromFS(&fileIO)) {
    Serial.println("UPDATE DONE");
    currentState = FIRMWARE_UPDATED;
    version->saveVersion(version->newFirmwareVersion());
    ESP.restart();
  } else {
    Serial.println("UPDATE FAILURE");
    delay(3000);
    currentState = SERVER_FOUND;
  }
  delete updater;
}
