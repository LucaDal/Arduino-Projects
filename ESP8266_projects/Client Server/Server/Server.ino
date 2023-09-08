#include <ESP8266WiFi.h>

//how many clients should be able to telnet to this ESP8266
#define MAX_SRV_CLIENTS 2
#define STACK_PROTECTOR 512  //per non intasare il cleint
const char* ssid = "Vodafone-A82448034";
const char* password = "2piedinidimoira";

WiFiServer server(50000);
WiFiClient serverClients[MAX_SRV_CLIENTS];

void setup() {

  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  uint8_t i = 0;
  while (WiFi.status() != WL_CONNECTED && i++ < 20) delay(1000);
  if (i == 21) {
    Serial.print("Could not connect to");
    Serial.println(ssid);
    while (1) delay(500);
  }
  server.begin();
  server.setNoDelay(true);

  Serial.print("Ready! Use 'telnet ");
  Serial.print(WiFi.localIP());
  Serial.println(" 50000' to connect");
}

void loop() {
  //check if there are any new clients
  uint8_t i;
  if (server.hasClient()) {
    for (i = 0; i < MAX_SRV_CLIENTS; i++) {
      if (!serverClients[i]) {  //Free space for client
        serverClients[i] = server.accept();
        Serial.print("New client: ");
        Serial.println(i);
        break;
      }
    }
    if (i == MAX_SRV_CLIENTS) {  //no free/disconnected spot so reject*
      server.accept().println("busy");
    }
  }
  
  //check TCP clients for data
  for (int i = 0; i < MAX_SRV_CLIENTS; i++)
    while (serverClients[i].available() && Serial.availableForWrite() > 0) {
      // working char by char is not very efficient
      Serial.write(serverClients[i].read());
    }

  // determine maximum output size "fair TCP use"
  // client.availableForWrite() returns 0 when !client.connected()
  //serve per inviare dati al client
  int maxToTcp = 0;
  for (int i = 0; i < MAX_SRV_CLIENTS; i++) {
    if (serverClients[i]) {
      int afw = serverClients[i].availableForWrite();
      if (afw) {
        if (!maxToTcp) {
          maxToTcp = afw;
        } else {
          maxToTcp = std::min(maxToTcp, afw);
        }
      } else {
        // warn but ignore congested clients
        Serial.println("one client is congested");
      }
    }
  }
  size_t len = std::min(Serial.available(), maxToTcp);
  len = std::min(len, (size_t)STACK_PROTECTOR);
  if (len) {
    uint8_t sbuf[len];
    int serial_got = Serial.readBytes(sbuf, len);
    // push UART data to all connected telnet clients
    for (int i = 0; i < MAX_SRV_CLIENTS; i++)
      // if client.availableForWrite() was 0 (congested)
      // and increased since then,
      // ensure write space is sufficient:
      if (serverClients[i].availableForWrite() >= serial_got) {
        size_t tcp_sent = serverClients[i].write(sbuf, serial_got);
        if (tcp_sent != len) { Serial.printf("len mismatch: available:%zd serial-read:%zd tcp-write:%zd\n", len, serial_got, tcp_sent); }
      }
  }
}