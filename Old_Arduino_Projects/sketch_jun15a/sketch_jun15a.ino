#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

const char *ssid = "FASTWEB - D'Alessandro";
const char *pass = "manzoni2018";

const char * host = "api.github.com";
const uint16_t port = 443;
const char * path = "https://rightintimedb.000webhostapp.com/request.php?device_name=8pklP";

void setup() {

Serial.begin(115200);

Serial.println("conntecting to network..");
WiFi.begin(ssid, pass);
while (WiFi.status() != WL_CONNECTED) {
delay(500);
Serial.print(".");
}
Serial.println("conntected to network..");

BearSSL::WiFiClientSecure client;
client.setInsecure();
HTTPClient https;

Serial.println("conntecting to server..");
if (https.begin(client, host, port, path)) {
  int httpsCode = https.GET();
  if (httpsCode > 0) {
    Serial.println(httpsCode);
    if (httpsCode == HTTP_CODE_OK) {
      String payload = https.getString();
      Serial.println(payload);
    }
  } else {
  Serial.print("failed to GET");
  }
} else {
  Serial.print("failed to connect to server");
}

}
void loop() {

}
