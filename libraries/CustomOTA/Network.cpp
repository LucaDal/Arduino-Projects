#include "Network.h"

const String API_KEY = "THIS_IS_MY_OWN_API_KEY";

Network::Network(const char * base_url){
  this->BASE_URL = base_url;
}

void Network::WiFiBegin(int EEPROMSize) {
  wifi = new MyWifiManager(EEPROMSize);
  Serial.println(wifi->connect());
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

Firmware Network::checkVersion() {

  Firmware firmware;
  firmware.build_num = -1;

  if ((WiFi.status() == WL_CONNECTED)) {

    String targetURL = BASE_URL;
    targetURL += "/api/get/version";
    http.begin(wifiClient,targetURL);

    if (http.GET() == HTTP_CODE_OK) {
      String payload = http.getString();
      Serial.println(payload);

      DeserializationError error = deserializeJson(doc, payload);
      if (error) {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.f_str());
        return firmware;
      }

      firmware.company = doc["companyName"].as<String>();
      firmware.build_num = doc["buildNum"].as<String>();
      firmware.build_date = doc["buildDate"].as<String>();
      firmware.server_file_path = doc["serverFilePath"].as<String>();
      firmware.file_size = doc["fileSize"];
      firmware.md5_checksum = doc["md5Checksum"].as<String>();

    } else {
      Serial.println("Error on HTTP request");
    }

    http.end();
  }

  return firmware;
}

bool Network::fileDownload(String target_path){
  String targetURL = BASE_URL;
  targetURL += "/api/post/update";
  if ((WiFi.status() == WL_CONNECTED)) {
    String httpRequestData = targetURL + "&api_key=" + API_KEY + "&target_path=" + target_path;
    MyUpdater update = MyUpdater();
    return update.startUpdate(wifiClient,httpRequestData);
  }
  return false;
}

