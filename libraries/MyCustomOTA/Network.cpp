#include "Network.h"

Network::Network(const char * base_url){
  this->BASE_URL = base_url;
}

void Network::WiFiBegin(int EEPROMSize) {
  wifi = new MyWifiManager(EEPROMSize);
  #ifdef DEBUG
    Serial.println(wifi->connect());
  #else
    wifi->connect();
  #endif
}

bool Network::isConnected(){
  if( WiFi.status() == WL_CONNECTED ){
    return true;
  }
  return false;
}

int Network::getFreeEEPROMAddress(){
  return wifi->getIndexEEPROM();
}

Firmware Network::checkVersion(String apy_key) {

  Firmware firmware;
  firmware.version = "-1";

  if ((WiFi.status() == WL_CONNECTED)) {

    String targetURL = BASE_URL;
    targetURL += "/api/get/version/" + apy_key;
    http.begin(wifiClient,targetURL);

    if (http.GET() == HTTP_CODE_OK) {
      String payload = http.getString();
      #ifdef DEBUG
        Serial.println(payload);
      #endif
      DeserializationError error = deserializeJson(doc, payload);
      if (error) {
        #ifdef DEBUG
          Serial.print(F("deserializeJson() failed: "));
          Serial.println(error.f_str());
        #endif
        return firmware;
      }
      firmware.version = doc["version"].as<String>();
      firmware.md5_checksum = doc["md5Checksum"].as<String>();
    } else {
      #ifdef DEBUG
        Serial.println("Error on HTTP request");
      #endif
    }

    http.end();
  }

  return firmware;
}

bool Network::fileDownload(String apy_key, String md5Checksum, String currentVersion){
  String httpRequestData = BASE_URL;
  httpRequestData += "/api/post/update/" + apy_key;
  if ((WiFi.status() == WL_CONNECTED)) {
    MyUpdater update = MyUpdater(md5Checksum);
    http.begin(wifiClient,httpRequestData);
    return update.startUpdate(http, currentVersion);
  }
  return false;
}

