#include <SimpleOTA.h>
#include <ESP8266HTTPClient.h>
#include <WiFiManager.h> 
#include <Certify.h>
SimpleOTA *simpleOTA = new SimpleOTA();
Certify cert("http://lucadalessandro.tech/domotica/api/get/fingerprint");
WiFiManager wifiManager;
const char* fingerPrint;

std::unique_ptr<BearSSL::WiFiClientSecure> client(new BearSSL::WiFiClientSecure);
HTTPClient https;

void setup() {
  Serial.begin(115200);
  wifiManager.setConfigPortalTimeout(120);
  wifiManager.autoConnect("Cancello");

  fingerPrint = cert.getFingerprint();

  simpleOTA->begin(512, "lucadalessandro.tech", fingerPrint, "WATER_PLANT");
}

void loop() {

}