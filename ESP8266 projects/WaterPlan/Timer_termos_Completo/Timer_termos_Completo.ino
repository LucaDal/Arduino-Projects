#include <ESP8266WiFi.h>
#include <NTPClient.h>
#include <ESP8266WebServer.h>
#include <SimpleOTA.h>
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
#define SPOT 12
#define NEON 13
#define TERM 15
#define OLED_RESET LED_BUILTIN

SimpleOTA *simpleOTA = new SimpleOTA();
Adafruit_SSD1306 display(OLED_RESET);
OneWire oneWire(ONE_WIRE_BUS);
RTC_DS1307 RTC;
DallasTemperature sensors(&oneWire);
WiFiClient client;

boolean neo;
boolean spo;
boolean ese = false;
boolean control;
boolean connection = false;
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
float t = 0.00f;
int i = 0;
char ssid[] = "";
char pass[] = "";

const long utcOffsetInSeconds = 7200;
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);

void setup() {

  Serial.begin(115200);
  simpleOTA->begin(512, "http://lucadalessandro.hopto.org:50001", "LUCA_CUBE_06");
  simpleOTA->checkUpdates(0);  //check instant the update
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  sensors.begin();
  Wire.begin();
  RTC.begin();
  pinMode(SPOT, OUTPUT);
  pinMode(NEON, OUTPUT);
  pinMode(TERM, OUTPUT);
  int cont = 0;
  while (control == false && cont < 7) {
    delay(500);
    ask();
    cont++;
  }
  if (control == false) {

    EEPROM.get(64, oraa1);
    EEPROM.get(65, mina1);
    EEPROM.get(66, oras1);
    EEPROM.get(67, mins1);
    EEPROM.get(68, oraa2);
    EEPROM.get(69, mina2);
    EEPROM.get(70, oras2);
    EEPROM.get(71, mins2);
    EEPROM.get(72, t);
  }
  Serial.print("\nora accensione1: ");
  Serial.println(oraa1);
  Serial.print("min accensione1: ");
  Serial.println(mina1);
  Serial.print("ora accensione2: ");
  Serial.println(oraa2);
  Serial.print("min accensione2: ");
  Serial.println(mina2);
  Serial.print("ora Spegnimento1: ");
  Serial.println(oras1);
  Serial.print("min spegnimento1: ");
  Serial.println(mins1);
  Serial.print("ora spegnimento2: ");
  Serial.println(oras2);
  Serial.print("min spegnimento2: ");
  Serial.println(mins2);
  Serial.print("temp: ");
  Serial.println(t);
  setOra();  //prende ora dal pool
}

void loop() {

  DateTime now = RTC.now();
  accendi(now);
  temp();
  draw(now);

  if (spo) {  //spo
    digitalWrite(SPOT, HIGH);
  } else {
    digitalWrite(SPOT, LOW);
  }
  if (neo) {  //neo
    digitalWrite(NEON, HIGH);
  } else {
    digitalWrite(NEON, LOW);
  }
  if (tempC < t) {
    digitalWrite(TERM, HIGH);
  } else {
    digitalWrite(TERM, LOW);
  }


  if ((_min % 1 == 0) && (ese == true)) {
    scrivi();
    //ask();
    ese = false;
  }

  if (now.second() < 1) {
    ese = true;
  }

  if (connection == false) {
    testConn();
  }
}
void setOra() {
  timeClient.begin();
  timeClient.update();
  RTC.adjust(DateTime(2019, 5, 29, timeClient.getHours(), timeClient.getMinutes(), timeClient.getSeconds()));
}


void draw(DateTime now) {
  String hours = String(_hour);
  String minss = String(_min);
  if (_hour < 10) {
    hours = "0" + hours;
  }
  if (_min < 10) {
    minss = "0" + minss;
  }

  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println(hours);
  display.setCursor(20, 0);
  display.println(":");
  display.setCursor(28, 0);
  display.println(minss);
  display.setCursor(0, 18);
  display.println(tempE);
  display.setCursor(63, 18);
  display.println("C");
  display.display();
}

void eewrite() {

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
bool scrivi() {

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
    std::unique_ptr<BearSSL::WiFiClientSecure> client(new BearSSL::WiFiClientSecure);
    client->setInsecure();
    HTTPClient https;
    Serial.print("[HTTPS] begin...\n");
    if (https.begin(*client, "https://dalessandroluca.altervista.org/Timer_termostato/send1810.php?device_name=8pklP&ora=" + hours + "&min=" + minss + "50&temp=" + tempE)) {  // HTTPS

      Serial.print("[HTTPS] GET...\n");
      Serial.print("https://dalessandroluca.altervista.org/Timer_termostato/send1810.php?device_name=8pklP&ora=" + hours + "&min=" + minss + "50&temp=" + tempE);
      int httpCode = https.GET();
      if (httpCode > 0) {
        if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
          Serial.printf("yeei scritto");
        }
      } else {
        Serial.printf("[HTTPS] GET... failed, error: %s\n", https.errorToString(httpCode).c_str());
        control = false;
      }

      https.end();
    } else {
      Serial.printf("[HTTPS] Unable to connect\n");
    }
  } else {
    connection = false;
    Serial.printf("[HTTPS] Unable to connect\n");
  }
}

void temp() {
  sensors.requestTemperatures();
  tempC = sensors.getTempCByIndex(0);
  if (tempC != DEVICE_DISCONNECTED_C) {
    tempE = String(tempC);
  } else {
    tempE = "Errore sensore";
  }
}




void accendi(DateTime now) {
  _hour = now.hour();
  _min = now.minute();

  if (oraa1 <= oras1) {
    if ((_hour >= oraa1) && (_hour <= oras1)) {
      spo = true;
      if ((_hour == oraa1) && (_min < mina1)) {
        spo = false;
      } else if ((_hour == oras1) && (_min >= mins1)) {
        spo = false;
      }
    } else {
      spo = false;
    }
  } else {  // oraa > oras
    if ((_hour >= oras1) && (_hour <= oraa1)) {
      spo = false;

      if ((_hour == oras1) && (_min < mins1)) {
        spo = true;
      } else if ((_hour == oraa1) && (_min >= mina1)) {
        spo = true;
      }
    } else {
      spo = true;
    }
  }

  //seconda presa - neon

  if (oraa2 <= oras2) {
    if ((_hour >= oraa2) && (_hour <= oras2)) {
      neo = true;
      if ((_hour == oraa2) && (_min < mina2)) {
        neo = false;
      } else if ((_hour == oras2) && (_min >= mins2)) {
        neo = false;
      }
    } else {
      neo = false;
    }
  } else {  // oraa > oras
    if ((_hour >= oras2) && (_hour <= oraa2)) {
      neo = false;

      if ((_hour == oras2) && (_min < mins2)) {
        neo = true;
      } else if ((_hour == oraa2) && (_min >= mina2)) {
        neo = true;
      }
    } else {
      neo = true;
    }
  }
}

void timerff() {
  //RTC.adjust(DateTime(2019,5,29, 0,29,30));
}

void ask() {
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
    std::unique_ptr<BearSSL::WiFiClientSecure> client(new BearSSL::WiFiClientSecure);
    client->setInsecure();
    HTTPClient https;
    Serial.print("[HTTPS] begin...\n");
    if (https.begin(*client, "https://dalessandroluca.altervista.org/Projects/requestFromTermostato.php?device_name=8pklP")) {  // HTTPS

      Serial.print("[HTTPS] GET...\n");
      int httpCode = https.GET();
      if (httpCode > 0) {
        if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
          String payload = https.getString();
          String a = getValue(payload, ';', 0);
          String b = getValue(payload, ';', 1);
          String c = getValue(payload, ';', 2);
          String d = getValue(payload, ';', 3);
          String e = getValue(payload, ';', 4);
          String f = getValue(payload, ';', 5);
          String g = getValue(payload, ';', 6);
          String h = getValue(payload, ';', 7);
          String temp = getValue(payload, ';', 8);
          oraa1 = a.toInt();
          mina1 = b.toInt();
          oras1 = c.toInt();
          mins1 = d.toInt();
          oraa2 = e.toInt();
          mina2 = f.toInt();
          oras2 = g.toInt();
          mins2 = h.toInt();
          t = temp.toFloat();
          //eewrite(); fare conrollo prima si acrivere -- con eeprom update
          control = true;
        }
      } else {
        Serial.printf("[HTTPS] GET... failed, error: %s\n", https.errorToString(httpCode).c_str());
        control = false;
      }

      https.end();
    } else {
      Serial.printf("[HTTPS] Unable to connect\n");
    }
  } else {
    connection = false;
    Serial.printf("[HTTPS] Unable to connect\n");
  }
}


String getValue(String data, char separator, int index) {
  int found = 0;
  int strIndex[] = { 0, -1 };
  int maxIndex = data.length() - 1;

  for (int i = 0; i <= maxIndex && found <= index; i++) {
    if (data.charAt(i) == separator || i == maxIndex) {
      found++;
      strIndex[0] = strIndex[1] + 1;
      strIndex[1] = (i == maxIndex) ? i + 1 : i;
    }
  }

  return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}
