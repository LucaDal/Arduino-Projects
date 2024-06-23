#include <SimpleOTA.h>
#include <WiFiManager.h>

#include <ESP8266HTTPClient.h>
#include <Certify.h>
#include <WebSocketClient.h>
#include "secret_data.h"

#define portone 2
#define cancelletto 0

SimpleOTA* simpleOTA = new SimpleOTA();
Certify cert("http://lucadalessandro.tech/domotica/api/get/fingerprint");
const char* fingerPrint;

void setup() {
  WiFiManager wifiManager;
  wifiManager.setConfigPortalTimeout(120);
  wifiManager.autoConnect("Cancello");
  
  pinMode(portone, OUTPUT);
  pinMode(cancelletto, OUTPUT);

  if (WiFi.status() == WL_CONNECTED) {
    fingerPrint = cert.getFingerprint();
    simpleOTA->begin(512, "lucadalessandro.tech", fingerPrint, MY_AUTH_TOKEN);
  }
}

void loop() {
  simpleOTA->checkUpdates(300); //ogni 5 minuti
  delay(2000);
  digitalWrite(cancelletto, LOW);
  digitalWrite(portone, HIGH);
  delay(2000);
  digitalWrite(portone, LOW);
  digitalWrite(cancelletto, HIGH);

}
