#ifndef NETWORK_H
#define NETWORK_H

#include <ArduinoJson.h>
#include <ESP8266HTTPClient.h>
#include "MyUpdater.h"
#include <WiFiManagerWithEEPROM.h>
#include "MyFirmware.h"

class Network {
private:
  HTTPClient http;
  WiFiClient wifiClient;
  StaticJsonDocument<300> doc;
  typedef void (*FuncPtrInt)(int);
  MyWifiManager *wifi;
  const char * BASE_URL;
  const char * target_path;
public:
  Network(const char *base_url);
  void WiFiBegin(int EEPROMSize);
  bool isConnected();
  int getFreeEEPROMAddress();
  Firmware checkVersion();
  bool fileDownload(String target_path);
};
#endif