#include <Wire.h>
#include <RTClib.h>
#include <LiquidCrystal.h>
 
#define BACKLIGHT 2
#define CONTRAST  3
 
#define SET       A0
#define PIU       A1
#define MENO      A2
 
char buffer[10];
unsigned long time=0;
unsigned long timeSet=0;
int  setModeTime=2000;
 
RTC_DS1307 RTC;
LiquidCrystal lcd(12, 11, 7, 6, 5, 4);
 
void setup () {
    pinMode( BACKLIGHT, OUTPUT );
    pinMode( CONTRAST, OUTPUT );
    digitalWrite( BACKLIGHT, HIGH );
    analogWrite( CONTRAST, 40 );
 
    Wire.begin();
    RTC.begin();
    lcd.begin(16, 2);
 
    if (! RTC.isrunning()) {
      Serial.println("RTC is NOT running!");
      RTC.adjust(DateTime(__DATE__, __TIME__));
    }
 
    pinMode( SET, INPUT );
    pinMode( PIU, INPUT );
    pinMode( MENO, INPUT );
}
 
void loop () {
 
    if ( analogRead( SET ) < 1000) { time = millis(); }
 
    DateTime now = RTC.now();
    lcd.clear();
 
    sprintf(buffer,  "%02d/%02d/%d", now.day(), now.month(), now.year());
    lcd.setCursor(0,0);
    lcd.print( buffer );
 
    char buffer[10] = "";
 
    sprintf(buffer,  "%02d:%02d:%02d", now.hour(), now.minute(), now.second());
    lcd.setCursor(0,1);
    lcd.print( buffer );
 
    if (time > 0 && setModeTime < (millis() - time) ) { setMode( now ); }
 
    delay(1000);
}
 
void setMode( DateTime now ) {
    boolean setMode = true;
    int setModeLevel = 0;
 
    int _day = now.day();
    int _month = now.month();
    int _year = now.year();
    int _hour = now.hour();
    int _min = now.minute();
    int _sec = now.second();
 
    lcd.clear();
    lcd.setCursor(0,0);
    sprintf(buffer,  "%s: %02d", "Giorno", _day);
    delay( 1000 );
    timeSet = millis();
 
    while ( setMode ) {
      if ( analogRead(SET) > 1000 || analogRead(PIU) > 1000 || analogRead(MENO) > 1000 ) { timeSet = millis(); }
 
      lcd.setCursor(0,0);
 
      // Set Day
      if ( setModeLevel == 0 ) {
        if ( analogRead(PIU) > 1000 && _day < 31) { _day++; }
        if ( analogRead(MENO) > 1000 && _day > 1) { _day--; }
 
        sprintf(buffer,  "%s: %02d", "Giorno", _day);
      }
      // Set Month
      if ( setModeLevel == 1 ) {
        if ( analogRead(PIU) > 1000 && _month < 12) { _month++; }
        if ( analogRead(MENO) > 1000 && _month > 1) { _month--; }
 
        sprintf(buffer,  "%s: %02d", "Mese", _month);
      }
      // Set Year
      if ( setModeLevel == 2 ) {
        if ( analogRead(PIU) > 1000 && _year < 9999) { _year++; }
        if ( analogRead(MENO) > 1000 && _year > 1900) { _year--; }
 
        sprintf(buffer,  "%s: %02d", "Anno", _year);
      }
      // Set Hour
      if ( setModeLevel == 3 ) {
        if ( analogRead(PIU) > 1000 && _hour < 24) { _hour++; }
        if ( analogRead(MENO) > 1000 && _hour > 1) { _hour--; }
 
        sprintf(buffer,  "%s: %02d", "Ora", _hour);
      }
      // Set Minute
      if ( setModeLevel == 4 ) {
        if ( analogRead(PIU) > 1000 && _min < 60) { _min++; }
        if ( analogRead(MENO) > 1000 && _min > 1) { _min--; }
 
        sprintf(buffer,  "%s: %02d", "Minuti", _min);
      }
      // Set Second
      if ( setModeLevel == 5 ) {
        if ( analogRead(PIU) > 1000 && _sec < 60) { _sec++; }
        if ( analogRead(MENO) > 1000 && _sec > 0) { _sec--; }
 
        sprintf(buffer,  "%s: %02d", "Secondi", _sec);
      }
 
      lcd.print( buffer );
      if ( analogRead(SET) > 1000 ) { lcd.clear(); setModeLevel++;  }
      if ( setModeLevel > 5 ) { setModeLevel=0; }
 
      if (timeSet > 0 && (setModeTime*2) < (millis() - timeSet) ) {
         RTC.adjust(DateTime(_year, _month, _day, _hour, _min, _sec));
         setMode = false;
      }
      delay(200);
    }
}
