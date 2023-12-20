#include <ESP8266WiFi.h>
bool greatings = false;
#define TIME_DELAY_TO_SEND_DATA 10000
#define TIMEOUT_RESPONSE 3000  //
#define PING_TIMER 40000       //
#define MAX_SRV_CLIENTS 2      // number of max client in this server (max are 5)
#define STACK_PROTECTOR 512    //per non intasare il cleint
const char* ssid = "Vodafone-A82448034";
const char* password = "2piedinidimoira";
WiFiServer server(50000);
WiFiClient serverClients[MAX_SRV_CLIENTS];

struct cube {
  uint8_t index;
  String name = "";
  String relatedTo = "";
  char faceToSend;
  bool needToSendData = false;
  bool needToSendNotification = false;
  bool pingingCube = false;
  long unsigned timeToRetry = 0;
  long unsigned timoutWaitingResponse = 0;
  long unsigned timerToPing;
  bool responseArrived = false;
};
cube cubes[MAX_SRV_CLIENTS];


void getCubeInfo(String value, uint8_t index) {
  uint8_t i = 1;
  bool divFound = false;
  char c;
  while (value.charAt(i) != '\0') {
    c = value.charAt(i);
    if (c == ':') {
      divFound = true;
      i++;
      c = value.charAt(i);
    }
    if (c == '&') {
      c = value.charAt(++i);
      if (c >= 'A' && c <= 'F') {
        faceArrived(index,&c);
      }
      break;
    }
    if (c != '\n') {
      if (divFound) {
        cubes[index].relatedTo += c;
      } else {
        cubes[index].name += c;
      }
    }
    i++;
  }
  Serial.println("cube setted:");
  Serial.print("name: ");
  Serial.println(cubes[index].name);
  Serial.print("releted to: ");
  Serial.println(cubes[index].relatedTo);
  Serial.print("char sent: ");
  Serial.print(cubes[index].faceToSend);
  Serial.println("");
}


void faceArrived(uint8_t i, char *c) {
  cubes[i].faceToSend = *c;
  cubes[i].needToSendData = true;
  cubes[i].timerToPing = millis() + PING_TIMER;
}


void resetCube(uint8_t i) {
  cubes[i].name = "";
  cubes[i].relatedTo = "";
  cubes[i].needToSendData = false;
  cubes[i].needToSendNotification = false;
  cubes[i].responseArrived = false;
  cubes[i].pingingCube = false;
  cubes[i].timeToRetry = 0;
  cubes[i].timoutWaitingResponse = 0;
  cubes[i].timerToPing = millis() + PING_TIMER;
}

/*
return the releted index of the cube;
-1 if not founed;
*/
int findIndexOfTheReletedCube(uint8_t i) {
  String nameRelated = cubes[i].relatedTo;
  //Serial.printf("nameRelated Of %s is %s\n", cubes[i].name.c_str(), nameRelated.c_str());
  if (nameRelated != "") {
    for (uint8_t cont = 0; cont < MAX_SRV_CLIENTS; cont++) {
      //if (cont != i){
      //Serial.printf("Trying if {%s} is equal to {%s} => %d\n", nameRelated.c_str(), cubes[cont].name.c_str(),nameRelated.equals(cubes[cont].name));
      //}
      if (cont != i && nameRelated.equals(cubes[cont].name)) {
        return cubes[cont].index;
      }
    }
  }
  return -1;
}

/*
index: cube that has to send data
indexReciver: cube that recive data and need to notify back
@parameters
*/
void writeToTheClient(uint8_t index, int indexReciver, char cahrToSend, bool isPing) {
  if (!cubes[indexReciver].needToSendNotification && serverClients[indexReciver].connected()) {  //CUBO A => B
    if (serverClients[indexReciver].availableForWrite() > 0) {
      serverClients[indexReciver].print(cahrToSend);
    }
    //Waiting the response B => SERVER
    cubes[indexReciver].needToSendNotification = true;
    if (!isPing) {
      cubes[index].needToSendData = false;
    } else {
      cubes[indexReciver].pingingCube = true;
    }
    cubes[indexReciver].timoutWaitingResponse = millis() + TIMEOUT_RESPONSE;
  }
}


void setup() {

  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  uint8_t i = 0;
  while (WiFi.status() != WL_CONNECTED && i++ < 20) delay(1000);
  if (i == 21) {
    Serial.print("Could not connect to");
    Serial.println(ssid);
    while (1) delay(500);
  }
  server.begin();
  server.setNoDelay(true);

  Serial.print("Ready! Use 'telnet ");
  Serial.print(WiFi.localIP());
  Serial.println(" 50000' to connect");
}

void loop() {
  //check if there are any new clients
  int8_t i;
  if (server.hasClient()) {
    for (i = 0; i < MAX_SRV_CLIENTS; i++) {
      if (!serverClients[i]) {  //Free space for client
        serverClients[i] = server.accept();
        cubes[i].index = i;
        Serial.print("New client: ");
        Serial.println(i);
        break;
      }
    }
    if (i == MAX_SRV_CLIENTS) {  //no free/disconnected spot so reject*
      server.accept().print("busy");
    }
  }

  //check cubes clients for data
  for (uint8_t i = 0; i < MAX_SRV_CLIENTS; i++) {
    String recived("");
    while (serverClients[i].available()) {
      recived += static_cast<char>(serverClients[i].read());
    }
    if (recived != "" && recived.length() < 20) {
      Serial.print("ricevuto: ");
      Serial.println(recived);
      char tempChar = recived.charAt(0);
      if (tempChar == '?') {
        resetCube(i);
        getCubeInfo(recived, i);
      } else if (tempChar == '!') {
        cubes[i].responseArrived = true;
      } else if (tempChar == 't') {  //for telnet
        resetCube(i);
        getCubeInfo(recived.substring(0, recived.length() - 2), i);
      } else if ((recived.length() == 1 || recived.length() == 3) && tempChar >= 'A' && tempChar <= 'F') {  //face recived
        Serial.printf("face: %c arrived\n", tempChar);
        faceArrived(i,&tempChar);
      }
    }
  }

  // A => SERVER => B => SERVER => A

  //contacts client
  for (int i = 0; i < MAX_SRV_CLIENTS; i++) {
    if (findIndexOfTheReletedCube(i) >= 0 && cubes[i].needToSendData && millis() > cubes[i].timeToRetry) {
      Serial.println("Sending face to the relative cube");
      writeToTheClient(i, findIndexOfTheReletedCube(i), cubes[i].faceToSend, false);
      Serial.println("done");
    }
  }


  // check for the response
  for (int i = 0; i < MAX_SRV_CLIENTS; i++) {           // i = related cube thah need to send the response
    if (cubes[i].needToSendNotification == true) {      // B recived and responded
      int indexReciver = findIndexOfTheReletedCube(i);  //indexReciver is the cube that has sent the face
      if (millis() < cubes[i].timoutWaitingResponse) {  //still in time to notify
        if (cubes[i].responseArrived) {
          cubes[i].needToSendNotification = false;
          cubes[i].responseArrived = false;
          if (!cubes[i].pingingCube) {  //se sto pingando => non ho bisogno di toccare l'altro cubo
            cubes[indexReciver].needToSendData = false;
            cubes[indexReciver].timeToRetry = 0;
            serverClients[indexReciver].print("OK");  //SERVER => A
          } else {
            cubes[i].pingingCube = false;
          }
        }
      } else {
        if (!cubes[i].pingingCube) {
          Serial.printf("impossible sending %c because the cube: %s doesn't responde\n",
                        cubes[indexReciver].faceToSend, cubes[i].name.c_str());

          cubes[indexReciver].needToSendData = true;
          cubes[indexReciver].timeToRetry = millis() + TIME_DELAY_TO_SEND_DATA;
        }
        Serial.printf("Closing connection with: %s", cubes[i].name.c_str());
        serverClients[i].flush();
        serverClients[i].stop();
        resetCube(i);
      }
    }
  }

  //ping client to see if they are connected -> otherwise disconnet
  for (static unsigned long pingTimer; millis() >= pingTimer; pingTimer = millis() + PING_TIMER) {
    for (int i = 0; i < MAX_SRV_CLIENTS; i++) {
      if (serverClients[i].connected() && millis() > cubes[i].timerToPing) {  //needed because it could just have stablieshed the connection
        //pingo solo se il relativo cubo non deve inviargli i dati
        if (!cubes[findIndexOfTheReletedCube(i)].needToSendData) {
          writeToTheClient(i, i, '?', true);
        }
      }
    }
  }
}