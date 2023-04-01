#include <ESP8266WiFi.h>   
#include <ESP8266WebServer.h>
#include <DNSServer.h>
#include <ESP8266mDNS.h>
#include <EEPROM.h>       
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <RTClib.h> 
#include <OneWire.h>
#include <DallasTemperature.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecureBearSSL.h>

#define ONE_WIRE_BUS 14
#define SPOT      12
#define NEON      13
#define TERM      15
#define OLED_RESET LED_BUILTIN  

Adafruit_SSD1306 display(OLED_RESET);
OneWire oneWire(ONE_WIRE_BUS);
RTC_DS1307 RTC;
DallasTemperature sensors(&oneWire);
WiFiClient client;  
 
boolean neo;
boolean spo;
boolean ese = false;
boolean control;
String tempE;
double tempC;
int oraa1; 
int mina1;
int oras1;
int mins1; 
int oraa2; 
int mina2; 
int oras2; 
int mins2; 
int _hour;
String hours;
int _min;
String minss;
double t=25.5;
int i=0;
int cont=0;

MDNSResponder mdns;
WiFiServer server(80);

const char* ssid = "BUBBLES";
String st;

void setup () {

  Serial.begin(115200);
  EEPROM.begin(1024);
  delay(10);
  Serial.println();
  Serial.println();
  Serial.println("Startup");
  // read eeprom for ssid and pass
  Serial.println("Reading EEPROM ssid");
  String esid;
  for (int i = 0; i < 32; ++i)
    {
      esid += char(EEPROM.read(i));
    }
  Serial.print("SSID: ");
  Serial.println(esid);
  Serial.println("Reading EEPROM pass");
  String epass = "";
  for (int i = 32; i < 96; ++i)
    {
      epass += char(EEPROM.read(i));
    }
  Serial.print("PASS: ");
  Serial.println(epass);  
  if ( esid.length() > 1 ) {
      // test esid 
      WiFi.begin(esid.c_str(), epass.c_str());
      if ( testWifi() == 20 ) { 
          launchWeb(0);
          return;
      }
  }
  setupAP(); 
}

int testWifi(void) {
  int c = 0;
  Serial.println("Waiting for Wifi to connect");  
  while ( c < 20 ) {
    if (WiFi.status() == WL_CONNECTED) { return(20); } 
    delay(500);
    Serial.print(WiFi.status());    
    c++;
  }
  Serial.println("Connect timed out, opening AP");
  return(10);
} 

void launchWeb(int webtype) {
          Serial.println("");
          Serial.println("WiFi connected");
          Serial.println(WiFi.localIP());
          Serial.println(WiFi.softAPIP());
          if (!mdns.begin("esp8266", WiFi.localIP())) {
            Serial.println("Error setting up MDNS responder!");
            while(1) { 
              delay(1000);
            }
          }
          Serial.println("mDNS responder started");
          // Start the server
          server.begin();
          Serial.println("Server started");   
          int b = 20;
          int c = 0;
          while(b == 20) { 
             b = mdns1(webtype);
           }
}

void setupAP(void) {
  
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);
  int n = WiFi.scanNetworks();
  Serial.println("scan done");
  if (n == 0)
    Serial.println("no networks found");
  else
  {
    Serial.print(n);
    Serial.println(" networks found");
    for (int i = 0; i < n; ++i)
     {
      // Print SSID and RSSI for each network found
      Serial.print(i + 1);
      Serial.print(": ");
      Serial.print(WiFi.SSID(i));
      Serial.print(" (");
      Serial.print(WiFi.RSSI(i));
      Serial.print(")");
      Serial.println((WiFi.encryptionType(i) == ENC_TYPE_NONE)?" ":"*");
      delay(10);
     }
  }
  Serial.println(""); 
  st = "<ul>";
  for (int i = 0; i < n; ++i)
    {
      // Print SSID and RSSI for each network found
      st += "<li>";
      st +=i + 1;
      st += ": ";
      st += WiFi.SSID(i);
      st += " (";
      st += WiFi.RSSI(i);
      st += ")";
      st += (WiFi.encryptionType(i) == ENC_TYPE_NONE)?" ":"*";
      st += "</li>";
    }
  st += "</ul>";
  delay(100);
  WiFi.softAP(ssid);
  Serial.println("softap");
  Serial.println("");
  launchWeb(1);
  Serial.println("over");
}

int mdns1(int webtype)
{
  // Check for any mDNS queries and send responses
  mdns.update();
  
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return(20);
  }
  Serial.println("");
  Serial.println("New client");

  // Wait for data from client to become available
  while(client.connected() && !client.available()){
    delay(1);
   }
  
  // Read the first line of HTTP request
  String req = client.readStringUntil('\r');
  
  // First line of HTTP request looks like "GET /path HTTP/1.1"
  // Retrieve the "/path" part by finding the spaces
  int addr_start = req.indexOf(' ');
  int addr_end = req.indexOf(' ', addr_start + 1);
  if (addr_start == -1 || addr_end == -1) {
    Serial.print("Invalid request: ");
    Serial.println(req);
    return(20);
   }
  req = req.substring(addr_start + 1, addr_end);
  Serial.print("Request: ");
  Serial.println(req);
  client.flush(); 
  String s;
  if ( webtype == 1 ) {
      if (req == "/")
      {
        IPAddress ip = WiFi.softAPIP();
        String ipStr = String(ip[0]) + '.' + String(ip[1]) + '.' + String(ip[2]) + '.' + String(ip[3]);
        s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>Hello from ESP8266 at ";
        s += ipStr;
        s += "<p>";
        s += st;
        s += "<form method='get' action='a'><label>SSID: </label><input name='ssid' length=32><input name='pass' length=64><input type='submit'></form>";
        s += "</html>\r\n\r\n";
        Serial.println("Sending 200");
      }
      else if ( req.startsWith("/a?ssid=") ) {
        // /a?ssid=blahhhh&pass=poooo
        Serial.println("clearing eeprom");
        for (int i = 0; i < 96; ++i) { EEPROM.write(i, 0); }
        String qsid; 
        qsid = req.substring(8,req.indexOf('&'));
        Serial.println(qsid);
        Serial.println("");
        String qpass;
        qpass = req.substring(req.lastIndexOf('=')+1);
        Serial.println(qpass);
        Serial.println("");
        
        Serial.println("writing eeprom ssid:");
        for (int i = 0; i < qsid.length(); ++i)
          {
            EEPROM.write(i, qsid[i]);
            Serial.print("Wrote: ");
            Serial.println(qsid[i]); 
          }
        Serial.println("writing eeprom pass:"); 
        for (int i = 0; i < qpass.length(); ++i)
          {
            EEPROM.write(32+i, qpass[i]);
            Serial.print("Wrote: ");
            Serial.println(qpass[i]); 
          }    
        EEPROM.commit();
        s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>Hello from ESP8266 ";
        s += "Found ";
        s += req;
        s += "<p> saved to eeprom... reset to boot into new wifi</html>\r\n\r\n";
      }
      else
      {
        s = "HTTP/1.1 404 Not Found\r\n\r\n";
        Serial.println("Sending 404");
      }
  } 
  else
  {
      if (req == "/")
      {
        s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>Hello from ESP8266";
        s += "<p>";
        s += "</html>\r\n\r\n";
        Serial.println("Sending 200");
      }
      else if ( req.startsWith("/cleareeprom") ) {
        s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>Hello from ESP8266";
        s += "<p>Clearing the EEPROM<p>";
        s += "</html>\r\n\r\n";
        Serial.println("Sending 200");  
        Serial.println("clearing eeprom");
        for (int i = 0; i < 96; ++i) { EEPROM.write(i, 0); }
        EEPROM.commit();
      }
      else
      {
        s = "HTTP/1.1 404 Not Found\r\n\r\n";
        Serial.println("Sending 404");
      }       
  }
  client.print(s);
  Serial.println("Done with client");
  return(20);

         display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
         sensors.begin();
         Wire.begin();
         RTC.begin();
         pinMode (SPOT,OUTPUT);
         pinMode (NEON,OUTPUT);
         pinMode (TERM,OUTPUT);
         ask();
         if (control == false && cont<10){
          ask();
          cont++;
        }
//        else{
//          oraa1 = (EEPROM.read(0) * 256) + EEPROM.read(1);
//          mina1 = (EEPROM.read(2) * 256) + EEPROM.read(3);
//          oras1 = (EEPROM.read(3) * 256) + EEPROM.read(4);
//          mins1 = (EEPROM.read(4) * 256) + EEPROM.read(5);
//          oraa2 = (EEPROM.read(6) * 256) + EEPROM.read(7);
//          mina2 = (EEPROM.read(8) * 256) + EEPROM.read(9);
//          oras2 = (EEPROM.read(10) * 256) + EEPROM.read(11);
//          mins2 = (EEPROM.read(12) * 256) + EEPROM.read(13);        
//        }
        Serial.println("ora accensione1");
        Serial.print(oraa1);
        Serial.print(":");
        Serial.print(mina1);
        Serial.println("ora accensione2");
        Serial.print(oraa2);
        Serial.print(":");
        Serial.print(mina2);
        Serial.println("ora Spegnimento");
        Serial.print(oras1);
        Serial.print(":");
        Serial.print(mins1);
        Serial.println("ora spegnimento");
        Serial.print(oras2);
        Serial.print(":");
        Serial.print(mins2);
}
 
void loop () {     
    DateTime now = RTC.now();
    accendi(now);
    temp();
    draw(now);
    
    if(spo){
      digitalWrite(SPOT, HIGH);
    }else{
      digitalWrite(SPOT, LOW);
    } 
    if(neo){
      digitalWrite(NEON, HIGH);
    }else{
      digitalWrite(NEON, LOW);
    }
    if(tempC < t){
      digitalWrite(TERM, HIGH);
    } else {
      digitalWrite(TERM, LOW);
    }
    
      if(i<=1){
      timerff();      //Solo in caso di prove
      i++;}

      if( (_min % 10 == 0) && (ese==true)){
        scrivi();
        ese = false;
      }
      if(now.second() <1){
        ese = true;
      }
//        ask();
//        if (control == false){
//          ask();
//        }
}

void draw(DateTime now){
    hours = String(_hour);
    minss = String(_min);
    if(_hour < 10){
      hours = "0"+hours;
    }
    if(_min < 10){
      minss = "0"+minss;
    }
    
    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(0,0);
    display.println(hours);
    display.setCursor(20,0);
    display.println(":");
    display.setCursor(28,0);
    display.println(minss);
    display.setCursor(0,18);
    display.println(tempE);
    display.setCursor(63,18);
    display.println("C");
    display.display();
    
}
void scrivi(){

      while (WiFi.status() != WL_CONNECTED) {
          display.clearDisplay();
          display.setTextSize(1);
          display.setTextColor(WHITE);         
          display.setCursor(0,0);
          display.println("Richiesta dati");
          display.display();
          Serial.print(".");
          delayMicroseconds(500000);
      }
       
      Serial.println("\nConnected to network");
  if ((WiFi.status() == WL_CONNECTED)) {
    std::unique_ptr<BearSSL::WiFiClientSecure>client(new BearSSL::WiFiClientSecure);
    client->setInsecure();
    HTTPClient https;
    Serial.print("[HTTPS] begin...\n");
    if (https.begin(*client, "http://dalessandroluca.altervista.org/Timer_termostato/send1810.php?device_name=8pklP&ora="+hours+"&min="+minss+"&temp="+tempE)) {  // HTTPS
    } 
    else {
      Serial.printf("[HTTPS] Unable to connect\n");
    }
  }

//      while (WiFi.status() != WL_CONNECTED) {
//          display.clearDisplay();
//          display.setTextSize(1);
//          display.setTextColor(WHITE);         
//          display.setCursor(0,0);
//          display.println("Salvataggio dati");
//          display.display();
//          Serial.print(".");
//          delay(500);
//      }
//       
//      Serial.println("\nConnected to network");
//
//      String query="INSERT INTO sql7291876.tempo_ora (oraSalvataggio,temp,cod_prodotto) VALUES ('"+hours+":"+minss+"','"+tempE+"','8pklP');";
//      int str_len = query.length() + 1; 
//      char INSERT_SQL[str_len];
//      query.toCharArray(INSERT_SQL, str_len);  
//      Serial.print(INSERT_SQL);
//      
//           Serial.print("Connecting to SQL...  ");
//         if (conn.connect(server_addr, 3306, user, password)){
//            Serial.println("OK.");
//            cursor->execute(INSERT_SQL);
//             conn.close();
//             Serial.println("connessione chiusa");
//          } else{
//          Serial.println("FAILED.");    
//         }
}

void temp() {
     sensors.requestTemperatures();
     tempC = sensors.getTempCByIndex(0);
     if(tempC != DEVICE_DISCONNECTED_C){
          tempE = String(tempC);
      } else{
          tempE = "Errore sensore";
      }
  }




    void accendi(DateTime now){
         _hour = now.hour();
         _min = now.minute();    
         if ( (_hour >= oraa1) && (_hour <= oras1) ) {
         spo = true;
            if ( (_hour == oraa1) && ( _min < mina1 ) ) {  
            spo = false;
            } else if ( (_hour == oras1) && ( _min >= mins1 ) ) {
               spo = false;
            } 
        }else{
        spo = false;
        }

         if ( (_hour >= oraa2 ) && (_hour <= oras2) ) {
         neo = true;
          if ( (_hour == oraa2) && ( _min < mina2 ) ) {  
            neo = false;
          }else if ( (_hour == oras2) && ( _min >= mins2) ) {
            neo = false;
          } 
        }else{
          neo = false;
        }
        
}

void timerff(){
      //RTC.adjust(DateTime(2019,5,29, 0,29,30));
     }

void ask(){
  while (WiFi.status() != WL_CONNECTED) {
          display.clearDisplay();
          display.setTextSize(1);
          display.setTextColor(WHITE);         
          display.setCursor(0,0);
          display.println("Richiesta dati");
          display.display();
          Serial.print(".");
          delayMicroseconds(500000);
      }
       
      Serial.println("\nConnected to network");
  if ((WiFi.status() == WL_CONNECTED)) {
    std::unique_ptr<BearSSL::WiFiClientSecure>client(new BearSSL::WiFiClientSecure);
    client->setInsecure();
    HTTPClient https;
    Serial.print("[HTTPS] begin...\n");
    if (https.begin(*client, "http://dalessandroluca.altervista.org/Timer_termostato/request1810.php?device_name=8pklP")) {  // HTTPS

          Serial.print("[HTTPS] GET...\n");
          int httpCode = https.GET();
          if (httpCode > 0) {
            if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
              String payload = https.getString();
              String a = getValue(payload,';',0);
              String b = getValue(payload,';',1);
              String c = getValue(payload,';',2);
              String d = getValue(payload,';',3);
              String e = getValue(payload,';',4);
              String f = getValue(payload,';',5);
              String g = getValue(payload,';',6);
              String h = getValue(payload,';',7);
              String temp = getValue(payload,';',8);
              oraa1 = a.toInt();
              mina1 = b.toInt();
              oras1 = c.toInt();
              mins1 = d.toInt();
              oraa2 = e.toInt();
              mina2 = f.toInt();
              oras2 = g.toInt();
              mins2 = h.toInt();
              double t= temp.toDouble();
              control=true;
            }
      } else {
        Serial.printf("[HTTPS] GET... failed, error: %s\n", https.errorToString(httpCode).c_str());
        control = false;
      }

      https.end();
    } else {
      Serial.printf("[HTTPS] Unable to connect\n");
    }
  }

}

String getValue(String data, char separator, int index)
{
  int found = 0;
  int strIndex[] = {0, -1};
  int maxIndex = data.length()-1;

  for(int i=0; i<=maxIndex && found<=index; i++){
    if(data.charAt(i)==separator || i==maxIndex){
        found++;
        strIndex[0] = strIndex[1]+1;
        strIndex[1] = (i == maxIndex) ? i+1 : i;
    }
  }

  return found>index ? data.substring(strIndex[0], strIndex[1]) : "";
}

void writeEEPROM(int startAdr, int laenge, char* writeString) {
  EEPROM.begin(1024); //Max bytes of eeprom to use
  yield();
  Serial.println();
  Serial.print("writing EEPROM: ");
  //write to eeprom 
  for (int i = 0; i < laenge; i++)
    {
      EEPROM.write(startAdr + i, writeString[i]);
      Serial.print(writeString[i]);
    }
  EEPROM.commit();
  EEPROM.end();           
}

void readEEPROM(int startAdr, int maxLength, char* dest) {
  EEPROM.begin(1024);
  delay(10);
  for (int i = 0; i < maxLength; i++)
    {
      dest[i] = char(EEPROM.read(startAdr + i));
    }
  EEPROM.end();    
  Serial.print("ready reading EEPROM:");
  Serial.println(dest);
}
  
    
