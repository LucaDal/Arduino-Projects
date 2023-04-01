
#define led1 1
#define led2 2
#define led3 3
#define led4 4
#define led5 5
#define led6 6
#define led7 7
#define led8 8
#define led9 9
#define led10 10
#define led11 11
#define led12 12
#define led13 13
#define p1 A2
#define p2 A0
int period = 500;
unsigned long time_now = 0;
short direct=3;
short led=7;
bool flag = true;
bool ran = true;
int score1;
int score2;
unsigned int turno = 0;

void setup() {
    Serial.begin(9600);
    pinMode(led1, OUTPUT);
    pinMode(led2, OUTPUT);
    pinMode(led3, OUTPUT);
    pinMode(led4, OUTPUT);
    pinMode(led5, OUTPUT);
    pinMode(led6, OUTPUT);
    pinMode(led7, OUTPUT);
    pinMode(led8, OUTPUT);
    pinMode(led9, OUTPUT);
    pinMode(led10, OUTPUT);
    pinMode(led11, OUTPUT);
    pinMode(led12, OUTPUT);
    pinMode(led13, OUTPUT);
    pinMode(p1, INPUT);
    pinMode(p2, INPUT);

}

void loop() {
  if(ran == true){
           direct = decidi_verso();
           short direct;
           digitalWrite(led7, HIGH);
  }
  
  if(direct == 1 && flag == true){ //true indica da 1 a 13 (->)
    led++;
    if(led == 14){
      direct = 0;
      led--;
    }
  }

  if(direct == 0  && flag == true){ //false indica da 13 a 1 (<-)
    led--;
    if(led == 0){
      direct = 1;
      led+=2;
    }
  }

  if( millis() > (time_now + period)) {
    time_now = millis();
    flag = true;
    switch(led){

      case 1: digitalWrite(led1,HIGH);
              digitalWrite(led2, LOW);
              break;
              
      case 2: digitalWrite(led1, LOW);
              digitalWrite(led2, HIGH);       
              digitalWrite(led3, LOW);
              break;
      case 3: digitalWrite(led2, LOW);
              digitalWrite(led3, HIGH);
              digitalWrite(led4, LOW);
              break; 
      case 4: digitalWrite(led3, LOW);
              digitalWrite(led4, HIGH);
              digitalWrite(led5, LOW);
              break;
      case 5: digitalWrite(led4, LOW);
              digitalWrite(led5, HIGH);
              digitalWrite(led6, LOW);
              break;
      case 6: digitalWrite(led5, LOW);
              digitalWrite(led6, HIGH);
              digitalWrite(led7, LOW);
              break;
      case 7: digitalWrite(led6, LOW);
              digitalWrite(led7, HIGH);
              digitalWrite(led8, LOW);
              break;
      case 8: digitalWrite(led7, LOW);
              digitalWrite(led8, HIGH);
              digitalWrite(led9, LOW);
              break;
      case 9: digitalWrite(led8, LOW);
              digitalWrite(led9, HIGH);
              digitalWrite(led10, LOW);
              break;
      case 10: digitalWrite(led9, LOW);
               digitalWrite(led10, HIGH);
               digitalWrite(led11, LOW);
               break;
      case 11: digitalWrite(led10, LOW);
               digitalWrite(led11, HIGH);
               digitalWrite(led12, LOW);
               break;
      case 12: digitalWrite(led11, LOW);
               digitalWrite(led12, HIGH);
               digitalWrite(led13, LOW);
               break; 
      case 13: digitalWrite(led12, LOW);
               digitalWrite(led13, HIGH);
               break;
    }
  }else{
    flag = false;
  }
  mossa();
}

short decidi_verso(){
  ran = false;
  short ris;
  short dir;
  ris = random(0, 10);
  Serial.println(ris);
  if (ris > 5){
    dir = 1;
  }else{
    dir = 0;
  }
  return dir;
}



void mossa(){
  
  int p1 = analogRead(p1);
  int p2 = analogRead(p2);
  Serial.print("analog read: ");
  Serial.println(p1);


  if(p1 > 900 && led == 12){
    if(led == 13){
      score1=2;
      period -= 100;
    }else{
     score1=1; 
     period -= 50;
    }
       
    direct = false;
    
  }
  if(p1 > 900 && led < 12){
    direct = false;   
  }
  if(p1 > 900 && period >600){
    period += 100;
  }

  
  if(p2 > 1 && led == 2){
    if(led == 1){
      period -= 100;
      score2=2;
    }else{
     score2=1; 
     period -= 50;
    }    
    direct = true;
    
  }
  if(p2 > 1 && led >2){
    direct = true;
    period += 100;
  }
    if(p2 > 1 && period >600){
    period += 100;
  }
}

void restart(){
  score1=0;
  score2=0;
  period=1000;
  ran=true;
}

void vincitore(){
  if(score1 == 10){
    digitalWrite(led1,HIGH);
    digitalWrite(led2, HIGH);
    digitalWrite(led3, HIGH);
    digitalWrite(led4, HIGH);
    digitalWrite(led5, HIGH);
    digitalWrite(led12, LOW);
    digitalWrite(led13, LOW);
    delay(500);
    digitalWrite(led2, LOW);
    digitalWrite(led3, LOW);
    digitalWrite(led4, LOW);
    digitalWrite(led5, LOW);
    digitalWrite(led1,HIGH);
    delay(500);
    digitalWrite(led1,LOW);
    delay(500);
    digitalWrite(led1,HIGH);
    delay(500);
    digitalWrite(led1,LOW);
    delay(1000);
    restart();
  }
  if(score2 == 10){
    digitalWrite(led13,HIGH);
    digitalWrite(led12, HIGH);
    digitalWrite(led11, HIGH);
    digitalWrite(led10, HIGH);
    digitalWrite(led9, HIGH);
    digitalWrite(led1, LOW);
    digitalWrite(led2, LOW);
    delay(500);
    digitalWrite(led9, LOW);
    digitalWrite(led12, LOW);
    digitalWrite(led11, LOW);
    digitalWrite(led10, LOW);
    digitalWrite(led13,HIGH);
    delay(500);
    digitalWrite(led13,LOW);
    delay(500);
    digitalWrite(led13,HIGH);
    delay(500);
    digitalWrite(led13,LOW);
    delay(1000);
    restart();
    }
}
