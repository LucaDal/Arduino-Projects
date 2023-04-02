#include <ArduinoJson.h>
#include <ESP8266HTTPClient.h>
#include "FileIO.h"
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

public:
  Network(const char *base_url);
  void WiFiBegin(int EEPROMSize);
  bool isConnected();
  int getFreeEEPROMAddress();
  Firmware checkVersion();
  String fileDownload(FileIO** fileIO, String target_path);
};
