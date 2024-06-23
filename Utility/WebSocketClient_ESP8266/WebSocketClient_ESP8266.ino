#include <ESP8266WiFi.h>
#include <WebSocketClient.h>

WebSocketClient webSocketClient;

// Use WiFiClient class to create TCP connections
WiFiClient client;

void setup() {
  Serial.begin(115200);
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to wifi");

  WiFi.begin("Vodafone-A82448034", "2piedinidimoira");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  connectWebSocket();
}


void connectWebSocket() {
  // Connect to the websocket server
  if (client.connect("lucadalessandro.tech",5001)) {
    Serial.println("Connected");
  } else {
    Serial.println("Connection failed. Retring in 5 seconds.");
    delay(5000);
    connectWebSocket();
  }

  webSocketClient.path = "/domotica/websocket";
  webSocketClient.host = "lucadalessandro.tech";
  if (webSocketClient.handshake(client)) {
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
  if (client.connected()) {
    webSocketClient.getData(data);
    if (data.length() > 0) {
      Serial.print("Received data: ");
      Serial.println(data);
      if (data != "accepted") {
        webSocketClient.sendData(String("ok"));
      }
    }
  } else {
    Serial.println("Client disconnected.");
    Serial.println("Retring in 5 seconds.");
    connectWebSocket();
  }
  // wait to fully let the client disconnect
  delay(1000);
}
