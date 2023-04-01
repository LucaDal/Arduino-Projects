#include <ArduinoJson.h>
#include <ESP8266HTTPClient.h>
#include "FileIO.h"
#include <WiFiManagerWithEEPROM.h>
#include "MyFirmware.h"

class Network {
private:
  HTTPClient http;
  StaticJsonDocument<300> doc;
  typedef void (*FuncPtrInt)(int);
  MyWifiManager *wifi;

public:
  void WiFiBegin();
  bool isConnected();
  int getFreeEEPROMAddress();
  Firmware checkVersion();
  String fileDownload(FuncPtrInt callback, FileIO** fileIO, String target_path);
};
