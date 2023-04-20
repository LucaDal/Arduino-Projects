#include "SimpleOTA.h"

static SimpleOTA *instance = NULL;

SimpleOTA::SimpleOTA() {
  #ifdef DEBUG
    Serial.println("starting the OTA client");
  #endif
  t1 = 0;
  instance = this;
}

void SimpleOTA::begin(int EEPROMSize,const char * base_url, const char * API_KEY) {
  this->initNetwork(EEPROMSize, base_url);
  this->initVersion();
  this->API_KEY = API_KEY;
}

/**
 * @brief check update every tot seconds
 * called from the main thread
 */
void SimpleOTA::checkUpdates(unsigned long seconds) {
    if (millis() - t1 >= seconds * 1000) {
      t1 = millis();
      if(network->isConnected()) {
        this->serverFirmwareCheck();
      } 
    }
}

void SimpleOTA::initVersion() {
  #ifdef DEBUG
    Serial.println("initVersion");
  #endif
  version = new FirmwareData(network->getFreeEEPROMAddress());
  #ifdef DEBUG
    Serial.printf("Current Version: %s\n",version->getNewFirmwareVersion().c_str());
  #endif
}

void SimpleOTA::initNetwork(int EEPROMSize, const char * base_url) {
  #ifdef DEBUG
    Serial.println("initNetwork");
  #endif
  network = new Network(base_url); 
  network->WiFiBegin(EEPROMSize);
}

void SimpleOTA::startDownload() {
  if(network->fileDownload(API_KEY,version->getFirmwareMD5Image(),version->getOldFirmwareVersion())){
    version->saveVersion(version->getNewFirmwareVersion());//save only if update goes fine
    #ifdef DEBUG
      Serial.println("Restarting");
    #endif
    delay(1000); // Wait a second and restart
    ESP.restart();
  }
}

void SimpleOTA::serverFirmwareCheck() {
  version->setNewFirmware(network->checkVersion(API_KEY));
  if (version->getNewFirmwareVersion() == "-1") {
    #ifdef DEBUG
      Serial.println("Server Not Responding");
    #endif
  } else {
    if (version->hasNewUpdate()) {
      #ifdef DEBUG
        Serial.println("New Build Available!");
        Serial.println("Starting the donwload!");
      #endif
      startDownload();
    }else{
      #ifdef DEBUG
        Serial.println("No new version");
      #endif
    }
  }
}


