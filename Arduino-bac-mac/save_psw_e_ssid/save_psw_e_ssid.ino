#include <EEPROM.h>

char wifi_ssid_private[32];
char wifi_password_private[32];
char clientName[10] = "newClient";
char ipAddr[16] = "172.024.001.001";//Pi Access Point IP-Adr.

//startAdr: offset (bytes), writeString: String to be written to EEPROM
void writeEEPROM(int startAdr, int laenge, char* writeString) {
  EEPROM.begin(512); //Max bytes of eeprom to use
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
  EEPROM.begin(512);
  delay(10);
  for (int i = 0; i < maxLength; i++)
    {
      dest[i] = char(EEPROM.read(startAdr + i));
    }
  EEPROM.end();    
  Serial.print("ready reading EEPROM:");
  Serial.println(dest);
}
  
void setup() {
  Serial.begin(9600);
  delay(100);
  
  strcat(wifi_ssid_private, "SSID1234");
  strcat(wifi_password_private, "PW1234");

  writeEEPROM(0,32,wifi_ssid_private);//32 byte max length
  writeEEPROM(32,32, wifi_password_private);//32 byte max length
  writeEEPROM(64,10, clientName);//10 byte max length
  writeEEPROM(74,16, ipAddr);//16 byte max length
  /*85 byte saved in total?*/  
  Serial.println("everything saved...");
  readEEPROM(0,32,wifi_ssid_private);
  readEEPROM(32,32,wifi_password_private);
  readEEPROM(64,10,clientName);
  readEEPROM(74,16,ipAddr);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("Loop");
  delay(1000);
}
