#include <Wire.h>
#include <RTClib.h>
#include "U8glib.h" 
#include <OneWire.h>
#include <DallasTemperature.h>
#define ONE_WIRE_BUS 10
#define SPOT      2
#define NEON      3
OneWire oneWire(ONE_WIRE_BUS);
RTC_DS1307 RTC;
DallasTemperature sensors(&oneWire);
U8GLIB_SSD1306_128X32 u8g(U8G_I2C_OPT_NONE);
boolean neo = false;
boolean spo = false;
String tempE;
double tempC;
int orasSpot=7;
int minsSpot=00;
int oraeSpot=20;
int mineSpot=30;
int _hour;
int _min;
int _sec;
double t = 25.5;
boolean adjustt = false;
 
void setup () {
      
 
      pinMode (SPOT,OUTPUT);
      pinMode (NEON,OUTPUT);

       
      Wire.begin();
      RTC.begin();
}
 
void loop () {
  
    DateTime now = RTC.now();
    accendi(now);
    
    
    
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
    
    
   // lcd.setCursor(0,1);
    
  //  lcd.print("spot: ");
  //  lcd.print(spo);
 //   lcd.print(" neon: ");
  //  lcd.print(neo);
    
    if(adjustt==false){
      timerff();
      adjustt= true;
    }
 
    
}





    void accendi(DateTime now){
         _hour = now.hour();
         _min = now.minute();
         _sec = now.second();

         if ( (_hour >= orasSpot) && (_hour <= oraeSpot) ) {
         spo = true;
            if ( (_hour == orasSpot) && ( _min < minsSpot ) ) {  
            spo = false;
            } else if ( (_hour == oraeSpot) && ( _min >= mineSpot ) ) {
               spo = false;
            } 
        }else{
        spo = false;
        }

         if ( (_hour >= 9 ) && (_hour <= 19) ) {
         neo = true;
      if ( (_hour == 9) && ( _min < 30 ) ) {  
        neo = false;
      }else if ( (_hour == 19) && ( _min >= 30) ) {
        neo = false;
      } 
    }else{
      neo = false;
    }
    
}

   void timerff(){
      //RTC.adjust(DateTime(2019,7,26, 1,36,10));
    }
    
