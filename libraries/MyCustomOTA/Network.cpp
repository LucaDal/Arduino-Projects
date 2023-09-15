#include "Network.h"

Network::Network(const char * base_url, const char * fingerPrint){
  this->BASE_URL = base_url;
  this->fingerPrint = fingerPrint;
}

void Network::WiFiBegin() {
  WiFi.setAutoReconnect(true);
  WiFi.persistent(true);
}

bool Network::isConnected(){
  if( WiFi.status() == WL_CONNECTED ){
    return true;
  }
  return false;
}

Firmware Network::checkVersion(String apy_key) {

  Firmware firmware;
  firmware.version = "-1";

  if (isConnected()) {

    client->setFingerprint(fingerPrint);
    String targetURL = "/ota/api/get/version/" + apy_key;
    const char* target = targetURL.c_str();

    if(https.begin(*client, BASE_URL, 443 ,target , true)){ //tls connection on port 443
      int httpCode = https.GET();
      if (httpCode == HTTP_CODE_OK) {
        String payload = https.getString();
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
          Serial.printf("[HTTPS] GET... failed, error: %s\n", https.errorToString(httpCode).c_str());
        #endif
      }
      https.end();
    }else{
        #ifdef DEBUG
          Serial.println("[HTTPS] Unable to connect\n");
        #endif
    }
  }
  return firmware;
}

bool Network::fileDownload(String apy_key, String md5Checksum, String currentVersion){
  String httpRequestData = "";
  httpRequestData += "/ota/api/post/update/" + apy_key;
  if ((WiFi.status() == WL_CONNECTED)) {
    MyUpdater update = MyUpdater(md5Checksum);
    if(https.begin(*client,BASE_URL,443, httpRequestData, true)){
      return update.startUpdate(https, currentVersion);
    }
  }
  return false;
}