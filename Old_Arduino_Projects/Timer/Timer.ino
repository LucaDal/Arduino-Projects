#include "Wire.h"
#include <LiquidCrystal.h>
#define DS3231_I2C_ADDRESS 0x68
#define Set 8 
#define Piu 9
#define Meno 10  
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

int val = 0;                
int stato = 0;

// Convert normal decimal numbers to binary coded decimal
byte decToBcd(byte val){
  return( (val/10*16) + (val%10) );
}
// Convert binary coded decimal to normal decimal numbers
byte bcdToDec(byte val){
  return( (val/16*10) + (val%16) );
}
void setup(){
  Wire.begin();
  pinMode(Set, INPUT);
  pinMode(Piu, INPUT);
  pinMode(Meno, INPUT);
  lcd.begin(16, 2);
  // set the initial time here:
  // DS3231 seconds, minutes, hours, date, month, year
  setDS3231time(30,42,16,13,10,16);
}
void setDS3231time(byte second, byte minute, byte hour, byte day, byte month, byte year){
  // sets time and date data to DS3231
  Wire.beginTransmission(DS3231_I2C_ADDRESS);
  Wire.write(0); // set next input to start at the seconds register
  Wire.write(decToBcd(second)); // set seconds
  Wire.write(decToBcd(minute)); // set minutes
  Wire.write(decToBcd(hour)); // set hours
  Wire.write(decToBcd(day)); // set date (1 to 31)
  Wire.write(decToBcd(month)); // set month
  Wire.write(decToBcd(year)); // set year (0 to 99)
  Wire.endTransmission();
}
void readDS3231time(byte *second,byte *minute,byte *hour,byte *day,byte *month,byte *year){
  Wire.beginTransmission(DS3231_I2C_ADDRESS);
  Wire.write(0); // set DS3231 register pointer to 00h
  Wire.endTransmission();
  Wire.requestFrom(DS3231_I2C_ADDRESS, 7);
  // request seven bytes of data from DS3231 starting from register 00h
  *second = bcdToDec(Wire.read() & 0x7f);
  *minute = bcdToDec(Wire.read());
  *hour = bcdToDec(Wire.read() & 0x3f);
  *day = bcdToDec(Wire.read());
  *month = bcdToDec(Wire.read());
  *year = bcdToDec(Wire.read());
}
void displayTime(){
  byte second, minute, hour, day, month, year;
  // retrieve data from DS3231
  readDS3231time(&second, &minute, &hour, &day, &month, &year);
  // send it to the serial monitor
  lcd.setCursor(0,0);
  Serial.print(hour, DEC);
  // convert the byte variable to a decimal number when displayed
  Serial.print(":");
  if (minute<10){
    Serial.print("0");
  }
  Serial.print(minute, DEC);
  Serial.print(":");
  if (second<10){
    Serial.print("0");
  }
  Serial.print(second, DEC);
  Serial.print(" ");
  Serial.print(day, DEC);
  Serial.print("-");
  Serial.print(month, DEC);
  Serial.print("-");
  Serial.println(year, DEC);
  
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

void loop(){
  displayTime(); // display the real-time clock data on the Serial Monitor,
  delay(1000); // every second
}
