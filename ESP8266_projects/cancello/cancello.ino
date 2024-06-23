#include <SimpleOTA.h>
#include <ESP8266HTTPClient.h>
#include <WiFiManager.h>
#include <Certify.h>
#include <WebSocketClient.h>
SimpleOTA* simpleOTA = new SimpleOTA();
Certify cert("http://lucadalessandro.tech/domotica/api/get/fingerprint");
WiFiManager wifiManager;

std::unique_ptr<BearSSL::WiFiClientSecure> client(new BearSSL::WiFiClientSecure);
HTTPClient https;
WiFiClient clientWebsocket;
WebSocketClient webSocketClient;
char *path = (char*)"/domotica/websocket";
char *host = (char*)"lucadalessandro.tech";
const char* fingerPrint;

void setup() {
  Serial.begin(115200);
  wifiManager.setConfigPortalTimeout(120);
  wifiManager.autoConnect("Cancello");

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
  }

  fingerPrint = cert.getFingerprint();
  simpleOTA->begin(512, "lucadalessandro.tech", fingerPrint, "WATER_PLANT");
  client->setFingerprint(fingerPrint);

  Serial.print("[HTTPS] begin...\n");
  if (https.begin(*client, "https://lucadalessandro.tech/domotica/api/connect/mytoken")) {  // HTTPS
    Serial.print("[HTTPS] GET...\n");
    int httpCode = https.GET();
    if (httpCode > 0) {
      Serial.printf("[HTTPS] GET... code: %d\n", httpCode);
      if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
        String payload = https.getString();
        Serial.println(payload);
      }
    } else {
      Serial.printf("[HTTPS] GET... failed, error: %s\n", https.errorToString(httpCode).c_str());
    }
    https.end();
  } else {
    Serial.printf("[HTTPS] Unable to connect\n");
  }
  connectWebSocket();
}

void connectWebSocket() {
  // Connect to the websocket server
  if (clientWebsocket.connect("lucadalessandro.tech", 5001)) {
    Serial.println("Connected");
  } else {
    Serial.println("Connection failed. Retring in 5 seconds.");
    delay(5000);
    connectWebSocket();
  }
  webSocketClient.path = path;
  webSocketClient.host = host;

  if (webSocketClient.handshake(clientWebsocket)) {
    Serial.println("Handshake successful");
    delay(1000);
    webSocketClient.sendData(String("mytoken"));
  } else {
    Serial.println("Handshake failed. Retring in 5 seconds.");
    delay(5000);
    connectWebSocket();
  }
}


void loop() {
  String data;
  if (clientWebsocket.connected()) {
    webSocketClient.getData(data);
    if (data.length() > 0) {
      Serial.print("Received data: ");
      Serial.println(data);
      if (data != "accepted") {
        webSocketClient.sendData(String("ok"));
      }
    }
  } else {
    clientWebsocket.stop();
    Serial.println("Client disconnected.");
    Serial.println("Retring in 5 seconds.");
    connectWebSocket();
  }
  // wait to fully let the client disconnect
  delay(1000);
}