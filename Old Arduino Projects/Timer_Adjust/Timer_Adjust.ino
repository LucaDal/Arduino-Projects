 
#include <Wire.h>
#include <RTClib.h>
#include <LiquidCrystal_I2C.h>
 
#define BACKLIGHT 2
#define CONTRAST  3
#define SPOT      14
#define NEON      13

 
char buffer[10];
int mN=9;


//int oan=10;
//int man=30;
//int mas=15;

boolean set=true; //true permette di modificare l'ora
 
RTC_DS1307 RTC;
LiquidCrystal_I2C lcd(0x27,16,2);
 
void setup () {
  
    pinMode( SPOT, OUTPUT);
    pinMode( NEON, OUTPUT);
//    pinMode( BACKLIGHT, OUTPUT );
//    pinMode( CONTRAST, OUTPUT );
//    digitalWrite( BACKLIGHT, HIGH );
//    analogWrite( CONTRAST, 40 );
    Serial.begin (9600);
    Wire.begin();
    RTC.begin();
//   lcd.begin();
    if (set){
      RTC.adjust(DateTime(30,05,2020,6,32,20));
    }
    if (! RTC.isrunning()) {
      Serial.println("RTC is NOT running!");
      RTC.adjust(DateTime(__DATE__, __TIME__));
    }
}
 
void loop () {
  
    int oas=8;
    int ends=2;//fino le 02:00
    DateTime now = RTC.now();
    start(now,&oas,&ends);
   // lcd.clear();
    
    //char buffer[10] = "";
 
   //sprintf(buffer,  "%02d:%02d:%02d", now.hour(), now.minute(), now.second());
   // lcd.setCursor(0,0);
   // lcd.println( buffer );
   // Serial.print(buffer);
}

 
void start(DateTime now, int *oas,int *ends){
    int _hour = now.hour();
    int _min = now.minute();
    int _sec = now.second();
    Serial.println (_hour);
    Serial.println(_min);
    //spot
  
    if(_hour<*oas && _hour>=*ends){
     // Serial.println("entro");
         digitalWrite(SPOT, HIGH); 
        }else{
           digitalWrite(SPOT, LOW);
        }
//Serial.println("-----------------");

    //neon
 //   while(_hour>=oan && _hour<=18){
//        if(_hour>=10 && _min<30){
        
  //      }else{
//          if(_hour>=18 && _min>=30){
 //         digitalWrite(NEON, LOW);
 //         }else{
 //         digitalWrite(NEON, HIGH);
   //       }
      
  //     }
    
//    }
     
}

             
