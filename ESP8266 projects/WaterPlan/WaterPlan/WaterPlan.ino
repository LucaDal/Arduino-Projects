#include <Wire.h>

#include <ESP8266HTTPClient.h>
HTTPClient http;
WiFiClient client;

#include "DFRobot_SHT20.h"
DFRobot_SHT20 sht20;

#include <EEPROM.h>

#include "DHT.h"
#define DHTTYPE DHT11
#define DHTPIN 3
DHT dht(DHTPIN, DHTTYPE);

#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define SCREEN_WIDTH 128  // OLED display width, in pixels
#define SCREEN_HEIGHT 32  // OLED display height, in pixels
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT);


#define MOTOR 1


#include "RTClib.h"
char daysOfTheWeek[7][12] = { "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday" };
RTC_DS1307 rtc;

#include <ESP8266WiFi.h>
const char *ssid = "Vodafone-A82448034";  //4
const char *password = "2piedinidimoira";

int umid_to_water = 0;
int ml_to_give = 0;
int millisec_to_adjust_water = 0;
int ora_a = 0;
int min_a = 0;

int next_update = 0;          //minute to send data
int cont_to_update_data = 0;  //ogni volta che invio dati aggiorno questo contatore
int minToSetFleg;
//irrigazione dati
int oraFineIrrigazione = 0;
const int intervallo = 10;


int hourInt;
int minInt;
String HA;
String TA;
int HT;
String TT;

bool annaffiato = true;
float millisWateringTime;
unsigned long timer = 0;
bool check = false;
bool checkAfterMilliliters = true;
bool waitToCheck = false;

unsigned long previousMillisData = 0;
const unsigned long intervalData = 10000;  //acquisition time and data
unsigned long previousMillisDisplay = 0;
unsigned long intervalDisplay = 2000;  //start displaing data for 2 second
bool displayState = false;
bool motorState = false;
bool dataIsRead = false;

void setup() {
  // inizializzo i sensori:
  Wire.begin(2, 0);//sda - scl
  WiFi.begin(ssid, password);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  sht20.initSHT20();
  delay(100);
  sht20.checkSHT20();
  EEPROM.begin(512);

  //GPIO 1 (TX) swap the pin to a GPIO.
  pinMode(MOTOR, FUNCTION_3);
  //GPIO 3 (RX) swap the pin to a GPIO.
  pinMode(DHTPIN, FUNCTION_3);
  //GPIO 1 (TX) swap the pin to a TX.
  //pinMode(1, FUNCTION_0);
  //GPIO 3 (RX) swap the pin to a RX.
  //pinMode(3, FUNCTION_0);
  pinMode(MOTOR, OUTPUT);
  dht.begin();

  int cont = 20;
  while ((WiFi.status() != WL_CONNECTED)) {
    display.clearDisplay();
    printOnScreen("Connection...", 2, 0, 0);
    delay(500);
    cont--;
    if (cont < 0) {
      break;
    }
  }
  if (WiFi.status() == WL_CONNECTED) {
    display.clearDisplay();
    printOnScreen("Connected", 2, 0, 0);
    delay(1000);
    askDataFromSiteAndUpdateEEPROM();  //Leggo i parametri dal sito
  } else {
    display.clearDisplay();
    printOnScreen("Error with WiFi", 1, 0, 0);
    readDataFromEEPROM();
    delay(2000);
  }


  display.clearDisplay();
  printOnScreen(String(umid_to_water), 2, 0, 0);
  printOnScreen("%", 2, 34, 0);
  printOnScreen(String(ml_to_give), 2, 50, 0);
  printOnScreen(String(millisec_to_adjust_water), 2, 0, 16);
  printOnScreen("|", 2, 55, 16);
  printOnScreen(String(ora_a), 2, 63, 16);
  printOnScreen("->", 1, 96, 22);
  printOnScreen(String(oraFineIrrigazione), 2, 108, 16);

  //Messaggio di benvenuto----------------
  delay(3000);
  display.clearDisplay();
  printOnScreen("Welcome", 2, 0, 0);
  display.clearDisplay();
  delay(1000);
  if (!rtc.begin()) {
    display.clearDisplay();
    printOnScreen("RTC error", 2, 0, 0);
    delay(2000);
  } else {
    display.clearDisplay();
    DateTime now = rtc.now();
    hourInt = now.hour();
    minInt = now.minute();
    printOnScreen(String(hourInt), 2, 0, 0);
    printOnScreen(":", 2, 25, 0);
    printOnScreen(String(minInt), 2, 55, 0);
    delay(2000);
    next_update = minInt;
  }
}

/*
will read the data from the db and it will update the interval time to water
*/
void updateIrrigationData() {
  millisWateringTime = (((ml_to_give - 150) * 200) / 5) + millisec_to_adjust_water;  // 20000/500 millesimi/ml -- ci ha messo 20 secondi per fare mezzo litro 150 valore correttivo temporaneo
  oraFineIrrigazione = ora_a + intervallo;
  if (oraFineIrrigazione >= 24) {
    oraFineIrrigazione -= 24;
  }
}


void printOnScreen(String text, int sizeText, int x, int y) {
  display.setTextSize(sizeText);
  display.setTextColor(WHITE);
  display.setCursor(x, y);
  display.println(text);
  display.display();
}


void sendDataToSite(String ora, String minn, String temp_aria, String temp_terreno, String umid_aria, String umid_terreno) {
  if ((WiFi.status() == WL_CONNECTED)) {
    Serial.print("[HTTPS] begin...\n");
    http.begin(client, "http://dalessandroluca.altervista.org/Projects/sentFromPlant.php?device_name=oJd4K&ora=" + ora + "&min=" + minn + "&temp_aria=" + temp_aria + "&temp_terreno=" + temp_terreno + "&umid_aria=" + umid_aria + "&umid_terreno=" + umid_terreno);
    http.GET();
    http.end();
  }
}


int getValureFromEEPROM(int *index) {
  char temp[] = "      ";
  for (int i = 0; i < 5; i++) {
    temp[i] = EEPROM.read(++(*index));
  }
  temp[5] = '\0';
  return atoi(temp);
}


void readDataFromEEPROM(void) {
  Serial.printf("leggo da EEPROM\n");
  int index = 0;
  umid_to_water = EEPROM.read(index);
  ml_to_give = getValureFromEEPROM(&index);
  millisec_to_adjust_water = getValureFromEEPROM(&index);
  ora_a = EEPROM.read(++index);
  min_a = EEPROM.read(++index);
  updateIrrigationData();
}


void writeStringToEEPROM(int *index, String str, bool *flag) {

  for (unsigned int i = 0; i < 5 - str.length(); i++) {  //max value is 99999
    if ('0' != EEPROM.read(++(*index))) {
      EEPROM.write(*index, '0');
      *flag = true;
    }
  }
  for (unsigned int i = 0; i < str.length(); i++) {
    if (str.charAt(i) != EEPROM.read(++(*index))) {
      EEPROM.write(*index, str.charAt(i));
      *flag = true;
    }
  }
}


void updateEEPROM(int *umid, int *ml, int *sec, int *oraa, int *mina) {
  bool flag = false;
  int index = 0;
  if (*umid != EEPROM.read(index)) {
    EEPROM.write(index, *umid);
    Serial.print("scrivo :umid\n");
    flag = true;
  }
  writeStringToEEPROM(&index, (String)*ml, &flag);
  writeStringToEEPROM(&index, (String)*sec, &flag);
  if (*oraa != EEPROM.read(++index)) {
    EEPROM.write(index, *oraa);
    flag = true;
  }
  if (*mina != EEPROM.read(++index)) {
    EEPROM.write(index, *mina);
    flag = true;
  }
  if (flag) {
    //Serial.print("Scrivo su eprom...\n");
    EEPROM.commit();
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


void askDataFromSiteAndUpdateEEPROM(void) {
  if ((WiFi.status() == WL_CONNECTED)) {
    //Serial.print("[HTTPS] begin...\n");
    if (http.begin(client, "http://dalessandroluca.altervista.org/Projects/requestFromPlant.php?device_name=oJd4K")) {
      //Serial.print("[HTTP] GET...\n");
      int httpCode = http.GET();
      if (httpCode > 0) {
        if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
          String payload = http.getString();
          umid_to_water = getValue(payload, ';', 0).toInt();
          ml_to_give = getValue(payload, ';', 1).toInt();
          millisec_to_adjust_water = getValue(payload, ';', 2).toInt();
          ora_a = getValue(payload, ';', 3).toInt();
          min_a = getValue(payload, ';', 4).toInt();
          //Serial.printf("letto: umid: %i,ml: %i,sec: %i,oraa: %i,mina: %i\n",umid,ml,sec,oraa,mina );
          updateEEPROM(&umid_to_water, &ml_to_give, &millisec_to_adjust_water, &ora_a, &min_a);
          updateIrrigationData();
        }
      }
    } else {
      display.clearDisplay();
      printOnScreen("Errore ricezzione dati", 1, 0, 0);
      delay(200);
    }
    http.end();
  }
}

/*
attivo il motorino per tot tempo dato da millisWateringTime,
nella seconda volta che entro spengo il motorino, segnalo dell'avvenuta irrigazione
e faccio eseguire un controllo sull'umidità solo dopo tot tempo impostato sotto settandowaiToCkeck a true
*/
void annaffia(unsigned long *currentMillis) {
  if (*currentMillis - timer >= millisWateringTime) {
    timer = *currentMillis;
    if (!motorState) {
      digitalWrite(MOTOR, HIGH);
      motorState = true;
    } else {
      digitalWrite(MOTOR, LOW);
      motorState = false;
      waitToCheck = true;  //mi server per il controllo sotto, altrimenti entra subito nel timer;
      annaffiato = true;
    }
  }
}

void loop() {
  unsigned long currentMillis = millis();

  //READING DATA---------------------------------------------------------------
  if (currentMillis - previousMillisData >= intervalData) {
    previousMillisData = currentMillis;
    if (!dataIsRead) {
      HA = String((int)dht.readHumidity());
      TA = String(dht.readTemperature());
      HT = ((int)sht20.readHumidity());
      TT = String(sht20.readTemperature());
      //RTC data
      DateTime now = rtc.now();
      hourInt = now.hour();
      minInt = now.minute();
      if (HT > 100) {  //è capitato che valesse 101
        HT = 100;
      }
      dataIsRead = true;
    } else {
      dataIsRead = false;
    }
  }


  //IRRIGAZIONE -----------------------------------------------------------
  if (!annaffiato) {
    if ((ora_a + intervallo) > 24) {
      if (((hourInt >= ora_a) && (hourInt <= 24)) || ((hourInt >= 0) && (hourInt <= oraFineIrrigazione)) || motorState) {
        annaffia(&currentMillis);
      }
    } else {
      if (((hourInt >= ora_a) && (hourInt <= oraFineIrrigazione)) || motorState) {
        annaffia(&currentMillis);
      }
    }
  }
  //controllo HT dopo tot tempo poichè necessita di un po di tempo per tornare sopra umid_to_water
  if (waitToCheck == false) {
    if (HT <= umid_to_water) {
      annaffiato = false;
    }
  } else {
    if (currentMillis - timer >= 120000) {  //due minuti  di attesa da appena finisco di irrigare
      waitToCheck = false;
    }
  }


  //DISPLAY----------------------------------------------------------------
  if (currentMillis - previousMillisDisplay >= intervalDisplay) {
    previousMillisDisplay = currentMillis;
    if (!displayState) {
      printOnScreen(HA, 2, 0, 0);
      printOnScreen("%", 2, 34, 0);
      printOnScreen(TA, 2, 64, 0);
      printOnScreen((String)HT, 2, 0, 16);
      printOnScreen("%", 2, 34, 16);
      printOnScreen(TT, 2, 64, 16);
      displayState = true;
      intervalDisplay = 2000;  //time data view
    } else {
      display.clearDisplay();
      display.display();
      intervalDisplay = 6000;  //time black screen
      displayState = false;
    }
  }


  // AGGIORNAMENTO DATI------------------------------------------------------------
  if (minInt == next_update) {
    next_update = minInt + 30;  //30 sono i minuti di attesa prima di aggiornare
    if (next_update > 59) {
      next_update -= 60;
    }
    cont_to_update_data += 1;
    if (cont_to_update_data >= 6) {  //ogni 3 ore aggiorno i dati poiche aggiorno ogni mezzora 12/2
      cont_to_update_data = 0;
      display.clearDisplay();
      printOnScreen("asking data", 2, 0, 0);
      askDataFromSiteAndUpdateEEPROM();
    }
    display.clearDisplay();
    printOnScreen("Sending data", 2, 0, 0);
    sendDataToSite(String(hourInt), String(minInt), TA, TT, HA, String(HT));
  }
}