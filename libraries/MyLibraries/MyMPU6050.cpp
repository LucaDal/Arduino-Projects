#include "MyMPU6050.h"
MPU6050 mpu;


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

//=========================================================================

MyMPU6050::MyMPU6050(uint8_t interruptPin){
  this->interruptPin = interruptPin;
}

/*
 supply your own gyro offsets here, scaled for min sensitivity use MPU6050_calibration.ino
                       XA    YA    ZA   XG   YG  ZG
*/
void MyMPU6050::begin(int XA, int YA, int ZA, int XG, int YG, int ZG) {
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
    MPUInitCntr++;
    #ifdef DEBUG
    String StatStr[5]{ "No Error", "initial memory load failed", "DMP configuration updates failed", "3", "4" };
    oled.print(F("MPU connection Try #"), 2, 0, 0);
    delay(1000);
    oled.print(String(MPUInitCntr), 2, 0, 0);
    delay(1000);
    oled.print(F("DMP Initialization failed (code "), 2, 0, 0);
    delay(1000);
    oled.print(StatStr[devStatus], 2, 0, 0);
    delay(1000);
    oled.print(F(")"), 2, 0, 0);
    #endif
    if (MPUInitCntr >= 10) return;  //only try 10 times
    delay(1000);
    begin(XA,YA,ZA,XG,YG,ZG);   //Lets try again
    return;
  }
  mpu.setXAccelOffset(XA);
  mpu.setYAccelOffset(YA);
  mpu.setZAccelOffset(ZA);
  mpu.setXGyroOffset(XG);
  mpu.setYGyroOffset(YG);
  mpu.setZGyroOffset(ZG);
  mpu.setDMPEnabled(true);
  attachInterrupt(interruptPin, dmpDataReady, RISING);
  mpuIntStatus = mpu.getIntStatus();
  // get expected DMP packet size for later comparison
  packetSize = mpu.dmpGetFIFOPacketSize();
  delay(1000);      // Let it Stabalize
  mpu.resetFIFO();  // Clear fifo buffer
  mpu.getIntStatus();
  mpuInterrupt = false;  // wait for next interrupt
}

void MPUMath(float *Y, float *P, float *R) {
  mpu.dmpGetQuaternion(&q, fifoBuffer);
  mpu.dmpGetGravity(&gravity, &q);
  mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);
  (*Y) = (ypr[0] * 180.0 / M_PI);
  (*P) = (ypr[1] * 180.0 / M_PI);
  (*R) = (ypr[2] * 180.0 / M_PI);
}

void getYPR(float *Y, float *P, float *R) {  // Best version I have made so far
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
    MPUMath(Y,P,R);  // <<<<<<<<<<<<<<<<<<<<<<<<<<<< On success MPUMath() <<<<<<<<<<<<<<<<<<<
  }
}

//====================================================================

void MyMPU6050::checkInterrupt(float *Y, float *P, float *R){
  if (mpuInterrupt){
    getYPR(Y,P,R);
  }
}