#include "Cube.h"
MyMPU6050 myMpu(RX);
PCF8574 pcf8574(0x20, 2, 0);


Cube::Cube(const char * THIS_CUBE_CODE,const char * RELATED_CUBE_CODE){
  this->THIS_CUBE_CODE = THIS_CUBE_CODE;
  this->RELATED_CUBE_CODE = RELATED_CUBE_CODE;
}

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

/*
This function will send to the server the face updated,
if it is connected ->  it will send only the face
if it has been disconnected -> will connected, present to the server and it will give the face
return false if cannot comunicate with the server
*/
bool Cube::updateServer() {
  if (!client.connected()) {            //se disconnesso mi collego
    if (!client.connect(host, port)) {  //se impossibile connettermi esco
      return false;
    } else {
      if (client.printf("%s:%s&%c", THIS_CUBE_CODE, RELATED_CUBE_CODE, faceToSend) != 0) {
        return true;
      } else {
        return false;
      }
    }
  } else{
    if (client.print(faceToSend) != 0) {
      return true;
    } 
  }
  return false;
}

void Cube::sendFace() {
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
}

/*
will connect to the server sending the cube info (without the face to send),
if it connectes will return true;
false otherwise
*/
bool Cube::connectToTheServer() {
  if (client.connect(host, port)) {
    client.printf("%s:%s", THIS_CUBE_CODE, RELATED_CUBE_CODE);
    return true;
  }
  return false;
}

void error(){
  pcf8574.digitalWrite(P2, HIGH);
  delay(1000);
  pcf8574.digitalWrite(P2, LOW);
  delay(1000);
}

void Cube::checkUpdateFromMPU() {
  if(!faceIsSetted){
    float Y, P, R;
    if (myMpu.checkInterrupt(&Y, &P, &R)){
      static long QTimer = millis();
      if ((long)( millis() - QTimer ) >= 500) { 
        QTimer = millis();
        faceToSend = getCubeFace(&Y, &P, &R);
        if (faceToSend != oldFace) {
          oldFace = faceToSend;
          contFaceSimilar = 0;
        } else{
          contFaceSimilar ++;
        }
        if(contFaceSimilar > 5){
          if (faceToSend == 'N') {
          while (1) {
            error();
          }
        }
          myMpu.setToSleep();
          faceIsSetted = true;
        }
      }
    }
  }
}

void Cube::checkConnection() {
  if (!client.connected()) {  // if disconnected ill try reconnecting every tot minute
    for (static unsigned long wait; (unsigned long)(millis() - wait) >= 300000; wait = millis()) {
      connectToTheServer();
    }
  }
}

void setPinMode() {
  pinMode(RX, FUNCTION_3);
  pinMode(RX, INPUT);
  //pinMode(TX, FUNCTION_3);
  //pinMode(TX, OUTPUT);
  //digitalWrite(TX, HIGH);
  pcf8574.pinMode(P2, OUTPUT);
  pcf8574.pinMode(P3, OUTPUT);
  pcf8574.pinMode(P4, OUTPUT);
  pcf8574.pinMode(P5, OUTPUT);
  pcf8574.pinMode(P6, OUTPUT);
  pcf8574.pinMode(P7, OUTPUT);
  pcf8574.digitalWrite(P2, LOW);
  pcf8574.digitalWrite(P3, LOW);
  pcf8574.digitalWrite(P4, LOW);
  pcf8574.digitalWrite(P5, LOW);
  pcf8574.digitalWrite(P6, LOW);
  pcf8574.digitalWrite(P7, LOW);
  }

void Cube::checkMessage(String serverMessage) {
  if (serverMessage == "OK\0") {  //the other cube recived the face
    dataIsSent = true;
  } else if (serverMessage == "busy\0") { //server has no more place to handle this client, try later
    secondToWaitBeforeSendingData = 40;
  } else if (serverMessage == "?\0") {  //Respond to server ping
    client.print("!\0");
  } else if (serverMessage[0] >= 'A' && serverMessage[0] <= 'F') {  //notify the reception
    client.print("!\0");
    if(serverMessage[0] == 'A') {pcf8574.digitalWrite(P2, HIGH);}
    if(serverMessage[0] == 'B') {pcf8574.digitalWrite(P3, HIGH);}
    if(serverMessage[0] == 'C') {pcf8574.digitalWrite(P4, HIGH);}
    if(serverMessage[0] == 'D') {pcf8574.digitalWrite(P5, HIGH);}
    if(serverMessage[0] == 'E') {pcf8574.digitalWrite(P6, HIGH);}
    if(serverMessage[0] == 'F') {pcf8574.digitalWrite(P7, HIGH);}
  }
}

void Cube::begin(int XA, int YA, int ZA, int XG, int YG,int ZG) {
  Wire.begin(2, 0);
  Wire.setClock(400000);
  setPinMode();
  myMpu.begin(XA, YA, ZA, XG, YG, ZG);
  connectToTheServer();
}

void Cube::start() {
  checkConnection();
  checkUpdateFromMPU();
  if (faceIsSetted && !dataIsSent) {
    sendFace();
  }
  if (client.available()) {
    String serverMessage = "";
    while (client.available()) {  // data recived from the server/other cube (passed through the server);
      serverMessage += static_cast<char>(client.read());
    }
    checkMessage(serverMessage);
  }
}