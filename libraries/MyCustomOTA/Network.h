#ifndef NETWORK_H
#define NETWORK_H

//#define DEBUG

#include <ArduinoJson.h>
#include <ESP8266HTTPClient.h>
#include "MyUpdater.h"
#include <WiFiManager.h> 
#include "MyFirmware.h"

class Network {
private:
  HTTPClient http;
  WiFiClient wifiClient;
  WiFiManager wifiManager;
  StaticJsonDocument<300> doc;
  typedef void (*FuncPtrInt)(int);
  const char * BASE_URL;

public:
  Network(const char *base_url);
  void WiFiBegin();
  bool isConnected();
  Firmware checkVersion(String apy_key);
  bool fileDownload(String apy_key, String md5Checksum, String currentVersion);
};
#endif