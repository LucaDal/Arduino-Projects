#include <ESP8266WiFi.h>   
#include <ESP8266WebServer.h>
#include <DNSServer.h>
#include <WiFiManager.h> 
#include <EEPROM.h>       
//#include <MySQL_Connection.h>
//#include <MySQL_Cursor.h>
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
//IPAddress server_addr(52,29,239,198);
//char user[] = "sql7291876";         
//char password[] = "r7uCzJiuX8";   

char ssid[]="FASTWEB - D'Alessandro";    
char pass[]="manzoni2018"; 
//char s[]="";    
//char p[]=""; 

WiFiClient client;   
//MySQL_Connection conn(&client);
//MySQL_Cursor* cursor;
//MySQL_Cursor cur = MySQL_Cursor(&conn);
 
void setup () {

     Serial.begin(115200);
     
           
     
     WiFi.mode(WIFI_STA);
     WiFi.begin(ssid, pass);
     while (WiFi.status() != WL_CONNECTED) {
          display.clearDisplay();
          display.setTextSize(2);
          display.setTextColor(WHITE);         
          display.setCursor(0,0);
          display.println("Connessione");
          display.display();
          Serial.print(".");
          delayMicroseconds(500000);
      }
//    if ((WiFi.status() != WL_CONNECTED)){
//          Serial.println("\n Starting");
//          unsigned long startedAt = millis();
//          WiFi.printDiag(Serial);
//          Serial.println("Opening configuration portal");
//          WiFiManager wifiManager;  
//          if (WiFi.SSID()!="") wifiManager.setConfigPortalTimeout(60);
//          if (!wifiManager.startConfigPortal("RightInTime")) {
//             Serial.println("Not connected to WiFi but continuing anyway.");
//          } else {
//             Serial.println("connected...yeey :)");
//             }
//        
//          Serial.print("After waiting ");
//          int connRes = WiFi.waitForConnectResult();
//          float waited = (millis()- startedAt);
//          Serial.print(waited/1000);
//          Serial.print(" secs in setup() connection result is ");
//          Serial.println(connRes);
//          if (WiFi.status()!=WL_CONNECTED){
//            Serial.println("failed to connect, finishing setup anyway");
//          } else{
//            Serial.print("local ip: ");
//            Serial.println(WiFi.localIP());
//          }
//          Serial.print("Stored SSID: ");
//          String sd = wifiManager.getSSID();
//          int len = sd.length() + 1; 
//          ssid[len];
//          sd.toCharArray(ssid, len);  
//          Serial.print(sd);
//          //writeEEPROM(0,25,ssid);
//          
//          Serial.print("Stored passphrase: ");
//          String ps = wifiManager.getPassword();
//          int len1 = ps.length() + 1; 
//          pass[len1];
//          ps.toCharArray(pass, len1); 
//          Serial.print(ps);
//          //writeEEPROM(25,25,pass);
//          
//         WiFi.mode(WIFI_STA);
//         // WiFi.begin("FASTWEB - D'Alessandro", "manzoni2018");
//         WiFi.begin(sd, ps); 
//                  
//         
//    }

//    cursor = new MySQL_Cursor(&conn);
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
      timerff();
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

//void writeEEPROM(int startAdr, int laenge, char* writeString) {
//  EEPROM.begin(512); //Max bytes of eeprom to use
//  yield();
//  Serial.println();
//  Serial.print("writing EEPROM: ");
//  //write to eeprom 
//  for (int i = 0; i < laenge; i++)
//    {
//      EEPROM.write(startAdr + i, writeString[i]);
//      Serial.print(writeString[i]);
//    }
//  EEPROM.commit();
//  EEPROM.end();           
//}

//void readEEPROM(int startAdr, int maxLength, char* dest) {
//  EEPROM.begin(512);
//  delay(10);
//  for (int i = 0; i < maxLength; i++)
//    {
//      dest[i] = char(EEPROM.read(startAdr + i));
//    }
//  EEPROM.end();    
//  Serial.print("ready reading EEPROM:");
//  Serial.println(dest);
//}
  
    
