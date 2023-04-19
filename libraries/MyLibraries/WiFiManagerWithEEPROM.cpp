#include "WiFiManagerWithEEPROM.h"

/* It will initialize the EEPROM*/
MyWifiManager::MyWifiManager(int byteEEPROM){
    EEPROM.begin(byteEEPROM);
}


void writeOnEEPROM(String value,int start){
    for (uint8_t i = 0; i < value.length(); ++i){
        EEPROM.write(start+i, value[i]);
    }
}


bool getConnectionReadingEEPROM(void){
    String esid = "";
    String epass = "";
    bool connected = false;
    for(uint8_t i = 0; i < 32; ++i){
        esid += char(EEPROM.read(i));
    }
    for(uint8_t i = 32; i < 64; ++i){
        epass += char(EEPROM.read(i));
    }
    //#ifdef DEBUG
        Serial.print("Starting wifi on: {");
        Serial.print(esid.c_str());
        Serial.print("} - password: {");
        Serial.print(epass.c_str());
        Serial.println("}");
    //#endif
    if (esid.length() > 1) {
        uint8_t tryall = 0;
        WiFi.begin(esid.c_str(), epass.c_str());
        //trying connection
        while (tryall <= 20) {
            if( WiFi.status() == WL_CONNECTED ){
                connected = true;
                break;
            }
            delay(1000);   
            tryall++;
        }

    }
    return connected;
}


String MyWifiManager::connect(void){
    bool newWifi = false;
    WiFi.mode(WIFI_STA);
    if(!getConnectionReadingEEPROM()){ 
        newWifi = true;
        //Opening configuration portal
        WiFiManager wifiManager; 
        //WiFi.printDiag(Serial); 
        if (WiFi.SSID()!="") wifiManager.setConfigPortalTimeout(120);
        if (!wifiManager.startConfigPortal("Domotica")) {
            return "Connection timeout... :<";
        }
        //Serial.print("After waiting ");
        WiFi.waitForConnectResult();
        uint8_t status = WiFi.status();
        if(status == WL_CONNECTED){
            //cleaning the eeprom
            for (uint8_t i = 0; i < 64; ++i) { EEPROM.write(i, 0); }
            //Saving SSID on EEPROM
            writeOnEEPROM(wifiManager.getWiFiSSID(),0);
            //Saving PASSWORD on EEPROM
            writeOnEEPROM(wifiManager.getWiFiPass(),32);
            EEPROM.commit(); 
        }else if (status == WL_NO_SSID_AVAIL){
            return "Invalid SSID";
        }else if(status == WL_CONNECT_FAILED){
            return "Connection failed";
        }
        else if(status == WL_WRONG_PASSWORD){
            return "Invalid password";
        }
    }
    WiFi.setAutoReconnect(true);
    WiFi.persistent(true);
    if(newWifi){
        return "New Network saved";
    }
    return "Connected to Network";
}


int MyWifiManager::getIndexEEPROM(void){
    return this->indexEEPROM;
}