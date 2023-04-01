/*************************************************************

  This is a simple demo of sending and receiving some data.
  Be sure to check out other examples!
 *************************************************************/
#define BLYNK_TEMPLATE_ID "TMPLM5LBxtv4"
#define BLYNK_DEVICE_NAME "Cancello casa"
#define BLYNK_AUTH_TOKEN "JtJOY20S6nQTGUT0zc8qGIRJkUfBm8ED"

//#define BLYNK_PRINT Serial

#include <arduino-timer.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "Vodafone - D Alessandro";
char pass[] = "manzoni2018";

auto timer = timer_create_default();

bool toggle_rele(void *) {
  if(digitalRead(2) == HIGH){
    digitalWrite(2, LOW);
    Blynk.virtualWrite(V0, 0);
  }
  return false; // repeat? true
}

// This function is called every time the Virtual Pin 0 state changes
BLYNK_WRITE(V0){
  int rele = param.asInt(); // Set incoming value from pin D2 to a variable
  if(rele){
    digitalWrite(2, HIGH);
    timer.in(1000,toggle_rele);
  }else{
    digitalWrite(2, LOW);
  }
}

// This function is called every time the device is connected to the Blynk.Cloud
BLYNK_CONNECTED()
{
  // Change Web Link Button message to "Congratulations!"
  //Blynk.setProperty(V3, "offImageUrl", "https://static-image.nyc3.cdn.digitaloceanspaces.com/general/fte/congratulations.png");
  //Blynk.setProperty(V3, "onImageUrl",  "https://static-image.nyc3.cdn.digitaloceanspaces.com/general/fte/congratulations_pressed.png");
  //Blynk.setProperty(V3, "url", "https://docs.blynk.io/en/getting-started/what-do-i-need-to-blynk/how-quickstart-device-was-made");
}

void setup()
{
  Serial.begin(115200);
  Blynk.begin(auth, ssid, pass);
  pinMode(2,OUTPUT);
  //Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);
  //Blynk.begin(auth, ssid, pass, IPAddress(192,168,1,100), 8080);
}

void loop()
{
  Blynk.run();
  timer.tick();
}
