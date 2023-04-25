#include "MyMPU6050.h"
MPU6050 mpu;

volatile bool mpuInterrupt = false;     // indicates whether MPU interrupt pin has gone high
void IRAM_ATTR dmpDataReady() { // for esp8266 add IRAM_ATTR
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

//=========================================================================

MyMPU6050::MyMPU6050(int interruptPin){
  this->interruptPin = interruptPin;
}

/*
 supply your own gyro offsets here, scaled for min sensitivity use MPU6050_calibration.ino
                       XA    YA    ZA   XG   YG  ZG
*/
void MyMPU6050::begin(int XA, int YA, int ZA, int XG, int YG, int ZG) {
  static int MPUInitCntr = 0;
  mpu.initialize(); // same
  devStatus = mpu.dmpInitialize();// same

  if (devStatus != 0) {
    // ERROR!
    // 1 = initial memory load failed
    // 2 = DMP configuration updates failed
    // (if it's going to break, usually the code will be 1)
    MPUInitCntr++;
    #ifdef  DEBUG
      Serial.print(F("MPU connection Try #"));
      Serial.println(MPUInitCntr);
      Serial.print(F("DMP Initialization failed (code "));
      Serial.print(devStatus);
      Serial.println(F(")"));
    #endif
    if (MPUInitCntr >= 10) return; //only try 10 times
    delay(1000);
    begin(XA, YA, ZA, XG, YG, ZG); // Lets try again
    return;
  }

  mpu.setXAccelOffset(XA);
  mpu.setYAccelOffset(YA);
  mpu.setZAccelOffset(ZA);
  mpu.setXGyroOffset(XG);
  mpu.setYGyroOffset(YG);
  mpu.setZGyroOffset(ZG);
  mpu.setDMPEnabled(true);
  #ifdef  DEBUG
  Serial.println(F("Enabling DMP..."));
  Serial.println(F("Enabling interrupt detection (Arduino external interrupt pin 2 on the Uno)..."));
  #endif
  attachInterrupt(interruptPin, dmpDataReady, RISING); 
  mpuIntStatus = mpu.getIntStatus(); // Same
  packetSize = mpu.dmpGetFIFOPacketSize();
  delay(1000); // Let it Stabalize
  mpu.resetFIFO(); // Clear fifo buffer
  mpu.getIntStatus();
  mpuInterrupt = false; // wait for next interrupt
}

void MPUMath(float *Y, float *P, float *R) {
  mpu.dmpGetQuaternion(&q, fifoBuffer);
  mpu.dmpGetGravity(&gravity, &q);
  mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);
  *Y = (ypr[0] * 180.0 / M_PI);
  *P = (ypr[1] * 180.0 / M_PI);
  *R = (ypr[2] * 180.0 / M_PI);
}

void getYPR(float *Y, float *P, float *R) {  // Best version I have made so far
  mpuInterrupt = false;
  FifoAlive = 1;
  fifoCount = mpu.getFIFOCount();
  uint16_t MaxPackets = 20;
  if ((fifoCount % packetSize) || (fifoCount > (packetSize * MaxPackets)) || (fifoCount < packetSize)) { // we have failed Reset and wait till next time!
    #ifdef  DEBUG
    Serial.println(F("Reset FIFO"));
    if (fifoCount % packetSize) Serial.print(F("\t Packet corruption")); // fifoCount / packetSize returns a remainder... Not good! This should never happen if all is well.
    Serial.print(F("\tfifoCount ")); Serial.print(fifoCount);
    Serial.print(F("\tpacketSize ")); Serial.print(packetSize);
    mpuIntStatus = mpu.getIntStatus(); // reads MPU6050_RA_INT_STATUS       0x3A
    Serial.print(F("\tMPU Int Status ")); Serial.print(mpuIntStatus , BIN);
    if (mpuIntStatus & B10000) { //FIFO_OFLOW_INT
      Serial.print(F("\tFIFO buffer overflow interrupt "));
    }
    if (mpuIntStatus & B1000) { //I2C_MST_INT
      Serial.print(F("\tSlave I2c Device Status Int "));
    }
    if (mpuIntStatus & B1) { //DATA_RDY_INT
      Serial.print(F("\tData Ready interrupt "));
    }
    Serial.println();
    #endif
    mpu.resetFIFO();// clear the buffer and start over
    mpu.getIntStatus(); // make sure status is cleared we will read it again.
  } else {
    while (fifoCount  >= packetSize) { // Get the packets until we have the latest!
      if (fifoCount < packetSize) break; // Something is left over and we don't want it!!!
      mpu.getFIFOBytes(fifoBuffer, packetSize); // lets do the magic and get the data
      fifoCount -= packetSize;
    }
    MPUMath(Y,P,R); // <<<<<<<<<<<<<<<<<<<<<<<<<<<< On success MPUMath() <<<<<<<<<<<<<<<<<<<
    if (fifoCount > 0) mpu.resetFIFO(); // clean up any leftovers Should never happen! but lets start fresh if we need to. this should never happen.
  }
}

void MyMPU6050::stopInterrupt(){
  detachInterrupt(interruptPin);
}

bool MyMPU6050::checkInterrupt(float *Y, float *P, float *R){
  if (mpuInterrupt ) { // wait for MPU interrupt or extra packet(s) available
    getYPR(Y,P,R); // Gets the MPU Data and canculates angles
    return true; //
  }
  return false;
}