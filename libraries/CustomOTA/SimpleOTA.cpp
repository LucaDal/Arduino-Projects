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
  version = new FirmwareData(network->getFreeEEPROMAddress());
  Serial.printf("Current Version: %s\n",version->getCurrentVersion().c_str());
}

void SimpleOTA::initNetwork(int EEPROMSize, const char * base_url) {
  Serial.println("initNetwork");
  network = new Network(base_url); 
  network->WiFiBegin(EEPROMSize);
}

void SimpleOTA::startDownload() {
  if(network->fileDownload(version->getFirmwareServerPath())){
    version->saveVersion(version->newFirmwareVersion());//save only if update goes fine
    Serial.println("Restarting");
    delay(1000); // Wait a second and restart
    ESP.restart();
  }
}

void SimpleOTA::serverFirmwareCheck() {
  version->setNewFirmware(network->checkVersion());
  if (version->newFirmwareVersion() == "-1") {
    Serial.println("Server Not Responding");
  } else {
    if (version->hasNewUpdate()) {
      Serial.println("New Build Available!");
      Serial.println("Starting the donwload!");
      startDownload();
    }
  }
}


