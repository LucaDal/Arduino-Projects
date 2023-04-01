#include <ESP8266WiFi.h>   
#include <NTPClient.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h> 
#include <WiFiUdp.h>
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
#define SPOT    12
#define NEON    13
#define TERM    15
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
boolean connection=false;
String tempE;
float tempC;
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
float t=0.00f;
int i=0;
char ssid[]="";    
char pass[]=""; 

const long utcOffsetInSeconds = 7200;
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);
 
void setup () {
      
      Serial.begin(115200);
      EEPROM.begin(1024);
      testConn(); 

    if ((WiFi.status() != WL_CONNECTED)){
          
          
          Serial.println("\n Starting");
          unsigned long startedAt = millis();
          WiFi.printDiag(Serial);
          Serial.println("Opening configuration portal");
          WiFiManager wifiManager;  
          if (WiFi.SSID()!="") wifiManager.setConfigPortalTimeout(50);
          if (!wifiManager.startConfigPortal("RightInTime")) {
             Serial.println("Not connected to WiFi but continuing anyway.");
          } else {
             Serial.println("connected...yeey :)");
            
           }
          
          Serial.print("After waiting ");
          int connRes = WiFi.waitForConnectResult();
          float waited = (millis()- startedAt);
          Serial.print(waited/1000);
          if (WiFi.status()!=WL_CONNECTED){
            Serial.println("failed to connect, finishing setup anyway");
          } else{
             Serial.println("clearing eeprom");
             for (int i = 0; i < 64; ++i) { EEPROM.write(i, 0); }
              Serial.print("vado a salvare SSID: ");
              String sd = wifiManager.getSSID();
              Serial.println("writing eeprom ssid:");
              for (int i = 0; i < sd.length(); ++i)
                {
                  EEPROM.write(i, sd[i]);
                  Serial.print("Wrote: ");
                  Serial.println(sd[i]); 
                }
              Serial.print("vado a salvare pass: ");
              String ps = wifiManager.getPassword();
               Serial.println("writing eeprom pass:"); 
              for (int i = 0; i < ps.length(); ++i)
                {
                  EEPROM.write(32+i, ps[i]);
                  Serial.print("Wrote: ");
                  Serial.println(ps[i]); 
                }    
               EEPROM.commit(); 
          }
        
         
    }
         display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
         sensors.begin();
         Wire.begin();
         RTC.begin();
         pinMode (SPOT,OUTPUT);
         pinMode (NEON,OUTPUT);
         pinMode (TERM,OUTPUT);
         int cont=0;
         while (control == false && cont<7){
          ask();
          cont++;
        }
        if (control == false){  
          
            EEPROM.get(64,oraa1);
            EEPROM.get(65,mina1);
            EEPROM.get(66,oras1);
            EEPROM.get(67,mins1); 
            EEPROM.get(68,oraa2); 
            EEPROM.get(69,mina2); 
            EEPROM.get(70,oras2); 
            EEPROM.get(71,mins2); 
            EEPROM.get(72,t);   

        }
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
        Serial.println("temp");
        Serial.print(t);
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
    
      if(i<=1 && connection == true){
      setOra();      //Solo in caso di prove
      i++;}

      if( (_min % 10 == 0) && (ese==true)){
        scrivi();
        ask();
        ese = false;
      }
 
      if(now.second() <1){
        ese = true;
      }

      if (connection==false) {
        testConn();    
      } 

}
void setOra(){
  timeClient.begin();
  timeClient.update();
  RTC.adjust(DateTime(2019,5,29, timeClient.getHours(),timeClient.getMinutes(),timeClient.getSeconds()));
}

void testConn(){
      delay(10);
      Serial.println();
      Serial.println("Startup");
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
          int c = 0;
          Serial.println("Waiting for Wifi to connect");  
          while ( c < 20 ) {
             if (WiFi.status() == WL_CONNECTED) {
              connection=true;
           
             } 
             delay(500);
             Serial.print(WiFi.status());    
             c++;
          }
      }  
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

void eewrite(){
   Serial.print("scrittura su eeprom");
            EEPROM.put(64, oraa1);
            EEPROM.put(65, mina1); 
            EEPROM.put(66, oras1);
            EEPROM.put(67, mins1); 
            EEPROM.put(68, oraa2);
            EEPROM.put(69, mina2); 
            EEPROM.put(70, oras2);
            EEPROM.put(71, mins2); 
            EEPROM.put(72, t); 
            EEPROM.commit(); 
}
void scrivi(){
//          WiFi.mode(WIFI_STA);
//          WiFi.begin(ssid, pass); 
//          while ((WiFi.status() != WL_CONNECTED)) {     
//          display.clearDisplay();
//          display.setTextSize(1);
//          display.setTextColor(WHITE);         
//          display.setCursor(0,0);
//          display.println("Richiesta dati");
//          display.display();
//          Serial.print(".");
//          delayMicroseconds(500000);
//          }

       
  if ((WiFi.status() == WL_CONNECTED)) {
    std::unique_ptr<BearSSL::WiFiClientSecure>client(new BearSSL::WiFiClientSecure);
    client->setInsecure();
    HTTPClient https;
    Serial.print("[HTTPS] begin...\n");
    if (https.begin(*client, "https://dalessandroluca.altervista.org/Timer_termostato/send1810.php?device_name=8pklP&ora="+hours+"&min="+minss+"&temp="+tempE)) {  
 
     }    
    else {
      Serial.printf("[HTTPS] Unable to connect\n");
    }
  } else{connection = false;}
  
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
//  while (WiFi.status() != WL_CONNECTED) {
//          display.clearDisplay();
//          display.setTextSize(1);
//          display.setTextColor(WHITE);         
//          display.setCursor(0,0);
//          display.println("Richiesta dati");
//          display.display();
//          Serial.print(".");
//          delayMicroseconds(500000);
//      }
       
  if ((WiFi.status() == WL_CONNECTED)) {
    std::unique_ptr<BearSSL::WiFiClientSecure>client(new BearSSL::WiFiClientSecure);
    client->setInsecure();
    HTTPClient https;
    Serial.print("[HTTPS] begin...\n");
    if (https.begin(*client, "https://dalessandroluca.altervista.org/Timer_termostato/request1810.php?device_name=8pklP")) {  // HTTPS

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
              t= temp.toFloat();
              eewrite();
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
  }else{
     connection = false;
     Serial.printf("[HTTPS] Unable to connect\n");}
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


  
    
