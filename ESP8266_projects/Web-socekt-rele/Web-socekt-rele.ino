#include <SimpleOTA.h>
#include <WiFiManager.h>

#include <ESP8266HTTPClient.h>
#include <Certify.h>
#include <WebSocketClient.h>
#include "secret_data.h"

#define cancelletto 0
#define OFF HIGH
#define ON LOW

SimpleOTA* simpleOTA = new SimpleOTA();
Certify cert("http://lucadalessandro.tech/domotica/api/get/fingerprint");
const char* fingerPrint;

void setup() {
  pinMode(cancelletto, OUTPUT);
  digitalWrite(cancelletto, OFF);

  WiFiManager wifiManager;
  wifiManager.setConfigPortalTimeout(120);
  wifiManager.autoConnect("Cancello");

  if (WiFi.status() == WL_CONNECTED) {
    fingerPrint = cert.getFingerprint();
    Serial.println("starting OTA");
    simpleOTA->begin(512, "lucadalessandro.tech", fingerPrint, MY_AUTH_TOKEN);
  }
}

void loop() {
  simpleOTA->checkUpdates(300); //ogni 5 minuti
  digitalWrite(cancelletto, ON);
  delay(1000);
  digitalWrite(cancelletto, OFF);
  delay(5000);
}
