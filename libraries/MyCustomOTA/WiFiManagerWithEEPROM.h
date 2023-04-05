#ifndef WIFIMANAGERWITHEEPROM_H
#define WIFIMANAGERWITHEEPROM_H

#include <WiFiManager.h> 
#include <EEPROM.h>    
#include <ESP8266WiFi.h>   
/*Used to connect to wifi dinamically.
If it doesn't connect it will open a portal where to insert parameters by a device,
if timeout expired then it will continue with ssid and password stored.

Connection restored if lost and persistent.
Other WiFi functions calling WiFi;
EEPROM occupies 32 byte for SSID and 32 for password total 64
*/
class MyWifiManager{
    private:
        int indexEEPROM = 64;
    public:
        /* It will initialize the EEPROM
            sizeEEPROM depends on the esp, for esp8266 is 512 
        */
        MyWifiManager(int sizeEEPROM);
        /*it'ill try connecting by parameters stored in EEPROM, otherwise
        it will open a portal where to connect using a device and it will
        store the parameters again
        @return: debug text
        */
        String connect(void);
        /*Returns the last index where to start writing*/
        int getIndexEEPROM(void);
};

#endif