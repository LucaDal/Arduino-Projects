#ifndef NETWORK_H
#define NETWORK_H

//#define DEBUG

#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecureBearSSL.h>
#include "MyUpdater.h"
#include "MyFirmware.h"

class Network {
private:
  HTTPClient https;
  std::unique_ptr<BearSSL::WiFiClientSecure> client {new BearSSL::WiFiClientSecure};
  StaticJsonDocument<300> doc;
  typedef void (*FuncPtrInt)(int);
  const char * BASE_URL;
  const char * fingerPrint;

public:
  Network(const char *base_url, const char * fingerPrint);
  void WiFiBegin();
  bool isConnected();
  Firmware checkVersion(String apy_key);
  bool fileDownload(String apy_key, String md5Checksum, String currentVersion);
};
#endif