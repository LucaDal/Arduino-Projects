#include <SSD1306.h>
#include "PCF8574.h"
#include "SimpleOTA.h"
#include "MyMPU6050.h"
SimpleOTA * simpleOTA  = new SimpleOTA();
MyMPU6050 mpu = new MyMPU6050();
SSD1306 oled(128,32);
WiFiClient client;
PCF8574 pcf8574(0x20,2,0);
#define RX 3
#define TX 1
#define TIME_DELAY_TO_RESEND_FACE 300000  //5 minuti
size_t contToSendData = 0;
size_t secondToWaitBeforeSendingData = 0;  //at startup
bool dataIsSent = false;
bool faceIsSetted = false;
uint8_t sendingToRelatedCube = 0;

//===============CUBE-INFO======================
const char *THIS_CUBE_CODE = "mimo";
const char *RELATED_CUBE_CODE = "luca";
//==============SERVER INFO=====================
const char *host = "lucadalessandro.hopto.org";
const uint16_t port = 50000;
//====================================================================

/*
return the face up of the cube and it will update the server if face is stabilized;
it will try 10 times to communicate with delay of 30 seconds,
if the face has arrived to the other cube then it will stop the communication
*/
char getCubeFace(float *Y, float *P, float *R) {
  char lastFace = 'A';
  if ((*R > -40 && *R < 40) && (*P < 40 && *P > -40)) {
    lastFace = 'A';
  } else if ((*R > 150 || *R < -150) && (*P < -140 || *P > 150)) {
    lastFace = 'F';
  } else if ((*P < -50 && *P >= -130) && ((*Y >= 40 && *Y <= 130) || *Y <= -60 || *Y >= 130 || (*Y <= 40 && *Y >= -60))) {
    lastFace = 'E';
  } else if ((*P >= 50 && *P <= 130) && ((*Y >= 30 && *Y <= 150) || (*P >= 50 && *P <= 130))) {
    lastFace = 'C';
  } else if ((*R >= -130 && *R <= -50) && ((*Y >= -70 && *Y <= 60) || (*Y >= 60 || *Y <= -140 || *Y <= 70))) {
    lastFace = 'D';
  } else if ((*R >= 50 && *R <= 120) && ((*Y >= -30 && *Y <= 150) || (*Y > 150 || *Y >= -150 || *Y <= -150))) {
    lastFace = 'B';
  } else {
    lastFace = 'N';
  }
  return lastFace;
}

//====================================================================

void sendFace() {
  for (static unsigned long SpamTimer; (unsigned long)(millis() - SpamTimer) >= 1000; SpamTimer = millis()) {
    contToSendData++;
    if (contToSendData > secondToWaitBeforeSendingData) {
      if (!updateServer()) {  //se fallisce => ritento tot minuti dopo
        secondToWaitBeforeSendingData = TIME_DELAY_TO_RESEND_FACE;
      } else {
        if (sendingToRelatedCube > 10) {  // after 10 times maybe the cube is offline => useless polling
          secondToWaitBeforeSendingData = TIME_DELAY_TO_RESEND_FACE;
        } else {
          secondToWaitBeforeSendingData = 30;
          if (sendingToRelatedCube <= 10) {
            sendingToRelatedCube++;
          }
        }
      }
      contToSendData = 0;
    }
  }
  oled.print(String(faceToSend));
}

//====================================================================
/*
This function will send to the server the face updated,
if it is connected ->  it will send only the face
if it has been disconnected -> will connected, present to the server and it will give the face
return false if cannot comunicate with the server
*/
bool updateServer() {
  oled.print("contacting server...");
  if (!client.connected()) {            //se disconnesso mi collego
    if (!client.connect(host, port)) {  //se impossibile connettermi esco
      return false;
    } else {
      if (client.printf("?%s:%s&%c", THIS_CUBE_CODE, RELATED_CUBE_CODE, faceToSend) != 0) {
        return true;
      } else {
        return false;
      }
    }
  }
  if (client.print(faceToSend) != 0) {
    return true;
  } else {
    return false;
  }
}

//====================================================================

int contTimes = 0;
void checkUpdateFromMPU(){
  if (digitalRead(TX) == HIGH) {// IT MEANS that the MPU is on
    float Y,P,R
    mpu.checkInterrupt(Y,P,R);
    for (static unsigned long SpamTimer; (unsigned long)(millis() - SpamTimer) >= 1000; SpamTimer = millis()) { //after a second i set the cube face
      char faceToSend = getCubeFace(&Y, &P, &R);
      digitalWrite(TX, LOW);
      oled.print(String(faceToSend));
      if (faceToSend == 'N') {
        digitalWrite(TX, LOW);
        while (1) {
          //Lampeggia led rosso =====================
        }
      }
      faceIsSetted = true;
    }
  }
}

//=====================================================================

void checkConnection() {
  if (!client.connected()) {  // if disconnected ill try reconnecting every tot minute
    for (static unsigned long wait; (unsigned long)(millis() - wait) >= 300000; wait = millis()) {
      connectToTheServer();
    }
  }
}

//=====================================================================
/*
will connect to the server sending the cube info (without the face to send),
if it connectes will return true;
false otherwise
*/
bool connectToTheServer() {
  if (client.connect(host, port)) {
    client.printf("?%s:%s", THIS_CUBE_CODE, RELATED_CUBE_CODE);
    oled.print("connected to server");
    return true;
  } else {
    oled.print("connection failed");
    return false;
  }
}


//====================================================================
void setup() {
  Wire.begin(2, 0);
  Wire.setClock(400000);
  delay(100);
  pinMode(RX, FUNCTION_3);
  pinMode(TX, FUNCTION_3);
  pinMode(RX, INPUT);
  pinMode(TX, OUTPUT);
  digitalWrite(TX, LOW);
  oled.initialize();
  oled.print("searching WiFi...");
  simpleOTA.begin(512,"http://192.168.1.12:9001","APY_TOKEN");//it includes WifiManager
  connectToTheServer();
  delay(1000);
  oled.print("Connection with mpu");
  digitalWrite(TX, HIGH);
	pcf8574.pinMode(P1, OUTPUT);
  pcf8574.pinMode(P2, OUTPUT);
  pcf8574.pinMode(P3, OUTPUT);
  pcf8574.pinMode(P4, OUTPUT);
  pcf8574.pinMode(P5, OUTPUT);
  pcf8574.pinMode(P6, OUTPUT);
  pcf8574.pinMode(P7, OUTPUT);
  delay(1000);
  MPU6050Connect();
  oled.print("Setup Complete...");
}

// ================================================================
// ===                         Loop                             ===
// ================================================================
String codePassed = "";
void loop() {
  simpleOTA->checkUpdates(10);
  checkUpdateFromMPU();
  if (digitalRead(TX) == HIGH) {
    if (mpuInterrupt) {  // wait for MPU interrupt or extra packet(s) available
      updateFromMPU();
      mpuLib.getYPR();
    }
  }
  if (faceIsSetted && !dataIsSent) {
    sendFace();
  }
  while (client.available()) {  // data recived from the server/other cube (passed through the server);
    codePassed += static_cast<char>(client.read());
  }
  if (codePassed != "") {        // DA modificare e accendere la facciata ritornata
    if (codePassed == "OK\0") {  //the other cube recived the face
      dataIsSent = true;
      oled.print("cube Recived face");
      delay(3000);
    } else if (codePassed == "busy\0") {  //server is busy
      oled.print("busy!");
      secondToWaitBeforeSendingData = 40;
      delay(3000);
    } else if (codePassed[0] >= 'A' && codePassed[0] <= 'F') {
      client.print("!\0");  //notify back
      oled.print("notifico la ricezione");
      delay(1000);
    } else if ("?\0") {     //ping from the server
      client.print("!\0");  //notify back
      delay(1000);
    }
    oled.print(codePassed);
    codePassed = "";
  }
  checkConnection();
}
