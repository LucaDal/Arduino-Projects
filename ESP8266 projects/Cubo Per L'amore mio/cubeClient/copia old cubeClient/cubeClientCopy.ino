#include "Wire.h"
#include "I2Cdev.h"
#include "MPU6050_6Axis_MotionApps20.h"
MPU6050 mpu;

//===========  My libreries  =================

#include <SSD1306.h>
#include "PCF8574.h"
#include "SimpleOTA.h"
SimpleOTA *simpleOTA;
SSD1306 oled;
MyWifiManager wifi(512);
WiFiClient client;
PCF8574 pcf8574(0x20,2,0);
#define RX 3
#define TX 1
#define TIME_DELAY_TO_RESEND_FACE 300000  //5 minuti
char lastFace = 'A';
char olderFace;
size_t contIfFaceHasChanged = 0;  //if major of 5 seconds in position than surface has changed
size_t contToSendData = 0;
size_t secondToWaitBeforeSendingData = 0;  //at startup
bool dataIsSent = false;
bool faceIsSetted = false;
uint8_t sendingToRelatedCube = 0;
char faceToSend;

//===============CUBE-INFO======================
const char *THIS_CUBE_CODE = "mimo";
const char *RELATED_CUBE_CODE = "luca";
//==============SERVER INFO=====================

const char *host = "lucadalessandro.hopto.org";
const uint16_t port = 50000;
//IPAddress myServer(192, 168, 137, 221);
//=======================================

// supply your own gyro offsets here, scaled for min sensitivity use MPU6050_calibration.ino
//                       XA    YA    ZA   XG   YG  ZG
int MPUOffsets[6] = { -3478, -129, 1808, 79, 13, 50 };

// ================================================================
// ===               INTERRUPT DETECTION ROUTINE                ===
// ================================================================
volatile bool mpuInterrupt = false;  // indicates whether MPU interrupt pin has gone high
void IRAM_ATTR dmpDataReady() {      //IRAM_ATTR needed for the esp8266
  mpuInterrupt = true;
}

// ================================================================
// ===                      MPU DMP SETUP                       ===
// ================================================================
int FifoAlive = 0;  // tests if the interrupt is triggering
int IsAlive = -20;  // counts interrupt start at -20 to get 20+ good values before assuming connected
// MPU control/status vars
uint8_t mpuIntStatus;    // holds actual interrupt status byte from MPU
uint8_t devStatus;       // return status after each device operation (0 = success, !0 = error)
uint16_t packetSize;     // expected DMP packet size (default is 42 bytes)
uint16_t fifoCount;      // count of all bytes currently in FIFO
uint8_t fifoBuffer[64];  // FIFO storage buffer
// orientation/motion vars
Quaternion q;         // [w, x, y, z]         quaternion container
VectorInt16 aa;       // [x, y, z]            accel sensor measurements
VectorInt16 aaReal;   // [x, y, z]            gravity-free accel sensor measurements
VectorInt16 aaWorld;  // [x, y, z]            world-frame accel sensor measurements
VectorFloat gravity;  // [x, y, z]            gravity vector
float euler[3];       // [psi, theta, phi]    Euler angle container
float ypr[3];         // [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector
byte StartUP = 100;   // lets get 100 readings from the MPU before we start trusting them (Bot is not trying to balance at this point it is just starting up.)

void MPU6050Connect() {
  static int MPUInitCntr = 0;
  // initialize device
  mpu.initialize();  // same
  // load and configure the DMP
  devStatus = mpu.dmpInitialize();  // same

  if (devStatus != 0) {
    // ERROR!
    // 1 = initial memory load failed
    // 2 = DMP configuration updates failed
    // (if it's going to break, usually the code will be 1)

    String StatStr[5]{ "No Error", "initial memory load failed", "DMP configuration updates failed", "3", "4" };

    MPUInitCntr++;

    oled.print(F("MPU connection Try #"), 2, 0, 0);
    delay(1000);
    oled.print(String(MPUInitCntr), 2, 0, 0);
    delay(1000);
    oled.print(F("DMP Initialization failed (code "), 2, 0, 0);
    delay(1000);
    oled.print(StatStr[devStatus], 2, 0, 0);
    delay(1000);
    oled.print(F(")"), 2, 0, 0);

    if (MPUInitCntr >= 10) return;  //only try 10 times
    delay(1000);
    MPU6050Connect();  // Lets try again
    return;
  }
  mpu.setXAccelOffset(MPUOffsets[0]);
  mpu.setYAccelOffset(MPUOffsets[1]);
  mpu.setZAccelOffset(MPUOffsets[2]);
  mpu.setXGyroOffset(MPUOffsets[3]);
  mpu.setYGyroOffset(MPUOffsets[4]);
  mpu.setZGyroOffset(MPUOffsets[5]);

  mpu.setDMPEnabled(true);
  attachInterrupt(RX, dmpDataReady, RISING);
  mpuIntStatus = mpu.getIntStatus();
  // get expected DMP packet size for later comparison
  packetSize = mpu.dmpGetFIFOPacketSize();
  delay(1000);      // Let it Stabalize
  mpu.resetFIFO();  // Clear fifo buffer
  mpu.getIntStatus();
  mpuInterrupt = false;  // wait for next interrupt
}


void GetDMP() {  // Best version I have made so far
  mpuInterrupt = false;
  FifoAlive = 1;
  fifoCount = mpu.getFIFOCount();
  if ((!fifoCount) || (fifoCount % packetSize)) {  // we have failed Reset and wait till next time!
                                                   //  digitalWrite(LED_PIN, LOW); // lets turn off the blinking light so we can see we are failing.
    mpu.resetFIFO();                               // clear the buffer and start over
  } else {
    while (fifoCount >= packetSize) {            // Get the packets until we have the latest!
      mpu.getFIFOBytes(fifoBuffer, packetSize);  // lets do the magic and get the data
      fifoCount -= packetSize;
    }
    MPUMath();  // <<<<<<<<<<<<<<<<<<<<<<<<<<<< On success MPUMath() <<<<<<<<<<<<<<<<<<<
  }
}

//====================================================================

/*
return the face up of the cube and it will update the server if face is stabilized;
it will try 10 times to communicate with delay of 30 seconds,
if the face has arrived to the other cube then it will stop the communication
*/
char getCubeFace(float *Y, float *P, float *R) {
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
void MPUMath() {
  float Y, P, R;
  mpu.dmpGetQuaternion(&q, fifoBuffer);
  mpu.dmpGetGravity(&gravity, &q);
  mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);
  Y = (ypr[0] * 180.0 / M_PI);
  P = (ypr[1] * 180.0 / M_PI);
  R = (ypr[2] * 180.0 / M_PI);
  for (static unsigned long SpamTimer; (unsigned long)(millis() - SpamTimer) >= 1000; SpamTimer = millis()) {
    faceToSend = getCubeFace(&Y, &P, &R);
    if (contTimes >= 3) {
      digitalWrite(TX, LOW);
      oled.print(String(faceToSend));
      faceIsSetted = true;
      if (faceToSend == 'N') {
        digitalWrite(TX, LOW);
        while (1) {
          //Lampeggia led rosso =====================
        }
      }
    } else {
      contTimes++;
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
  simpleOTA = new SimpleOTA();
  simpleOTA->begin(512,"http://192.168.1.12:9001","APY_TOKEN");//it includes WifiManager
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
  
  if (digitalRead(TX) == HIGH) {
    if (mpuInterrupt) {  // wait for MPU interrupt or extra packet(s) available
      GetDMP();
    }
  }
  if (faceIsSetted && !dataIsSent && faceToSend != 'N') {
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
