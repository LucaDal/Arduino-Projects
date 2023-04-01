#include <ESP8266WiFi.h>
#include <SPI.h>

//creo un client che punta al
//servizio dynupdate.no-ip.com sulla porta 80
WiFiClient client;
const char* ssid = "XMG-LUCA 6464";
const char* password = "123456789";
#define INTERVAL_UPDATE 600000 //10 minuti

void setup() {
  //Avvio la connessione
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  //Serial.begin(115200);
  delay(1000);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  //Serial.println("Connessione in corso…");
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    if (client.connect("dynupdate.no-ip.com", 80)) {
      //Serial.println("Richiesta");
      client.println("GET /nic/update?hostname=lucadalessandro.hopto.org HTTP/1.0");
      client.println("Host: dynupdate.no-ip.com");
      client.println("Authorization: Basic bHVjYWRhbGU6THVjYURhbGUwMA==");  //lucadale:LucaDale00 in base64
      client.println("User-Agent: esp8266/1.0 luca-dalessandro@virgilio.it");
      client.println();
    } else {
      //Problemi nella connessione
      Serial.println("connessione fallita!");
    }
    //Serial.println("Disconessione");
    client.flush();
    client.stop();
      //ciclo infinito
    delay(INTERVAL_UPDATE);
  }
  delay(2000);
  //Quando il server si disconnette fermo il client
}