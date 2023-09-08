// assigning arduino digital pins for the various led display pins
int pinA = 3;
int pinB = 7;
int pinC = 12;
int pinD = 10;
int pinE = 9;
int pinF = 4;
int pinG = 13;
int pinDP = 11; // the decimal point pin
int D1 = 2;
int D2 = 5;
int D3 = 6;
int D4 = 8;
int t = 250; // time delay
int const button1 = A3;
int const button2 = A4;
int buttonState1 = 0;
int buttonState2 = 0;
bool myLoop = true;

/* GAME ATTRIBUTES */

bool mode = false; //false = 11; true 21;
int player1D1 = 0;
int player1D2 = 0;
int player2D3 = 0;
int player2D4 = 0;
bool battitore; //true = p1; false = p2;
bool flag;
int cont;
int nextChange = 0;
long interval = 1000; //intervallo per stampare "Fine" 
int ledState = LOW;
long previousMillis = 0;  

// SCORES
int final11_1 = 1;
int final11_2 = 1;

int final21_1 = 2;
int final21_2 = 1;
void setup(){
  Serial.begin(9600);              
  // initialise the digital pins as outputs.
  
  pinMode(button1, INPUT_PULLUP);
  pinMode(button2, INPUT_PULLUP);
  pinMode(pinA, OUTPUT);     
  pinMode(pinB, OUTPUT);     
  pinMode(pinC, OUTPUT);     
  pinMode(pinD, OUTPUT);     
  pinMode(pinE, OUTPUT);     
  pinMode(pinF, OUTPUT);     
  pinMode(pinG, OUTPUT);   
  pinMode(pinDP, OUTPUT);  
  pinMode(D1, OUTPUT);  
  pinMode(D2, OUTPUT);  
  pinMode(D3, OUTPUT);  
  pinMode(D4, OUTPUT);  
}

void loop() { 

     mySetup();
      
     while(myLoop){
      buttonState1 = analogRead(button1);
      buttonState2 = analogRead(button2);

      /*       AGGIORNO PUNTEGGIO PPLAYER           */
      if(buttonState1 < 500){ //incremento score p1
        player1D2 ++;
        if(player1D2 >9){
          player1D1 ++;
          player1D2 = 0;
        }
       print_off();
       check();
       digitalWrite(pinG, HIGH);
       delay(500);
      }

      if(buttonState2 < 500){ //incremento score p2
        player2D4 ++; 
        if(player2D4 > 9){
          player2D3 ++;
          player2D4 = 0;
        }
       print_off();
       check();
       digitalWrite(pinG, HIGH); //trattino
       delay(500);
      }

      /*    BATTUTA     */
        if((player1D2 > player2D4 || player1D2 < player2D4) && flag){
         
           flag = false;
           cont = 0;
           firstPoint();
        }

      /*     AGGIORNO IL DISPLAY     */

      if(flag){ //IF FLAG = TRUE PRINT Batt ON DISPLAY
        write_D1();
        print_b();
        delay(1);
        write_D2();
        print_a();
        delay(1);
        write_D3();
        print_I();
        delay(1);
        write_D4();
        print_I();
        delay(1);
      }else{
      write_D1();
      Display(player1D1);
      delay(1);
      write_D2();
      Display(player1D2);
      delay(1);

      if(battitore && !flag) { //battitore true == p1;
         write_D1();
         print_off();
         print_decimal();
         delay(1);
         write_D2();
         print_off();
         print_decimal();
         delay(1);
      }else if(!flag){
         write_D3();
         print_off();
         print_decimal();
         delay(1);
         write_D4();
         print_off();
         print_decimal();
         delay(1);
      }
      
      write_D3();
      Display(player2D3);
      delay(1);
      write_D4();
      Display(player2D4);
      delay(1);      
      }
   }
}

void mySetup(){
  flag = true; //inizializo per la prima battuta
  ledState = LOW;
  player1D1 = 0;
  player1D2 = 0;
  player2D3 = 0;
  player2D4 = 0;
  cont = 0; // contatore per cambiare chi batte
  selectMode();
  myLoop = true;
  
}


void winner(bool fine){ //player = true means p1;
  delay(500);
  while(fine){
        buttonState1 = analogRead(button1);
        buttonState2 = analogRead(button2);
  
        unsigned long currentMillis = millis();
   
    if(currentMillis - previousMillis > interval) {
         previousMillis = currentMillis;   
   
        if (ledState == LOW){
        ledState = HIGH;
        }else{
        ledState = LOW;
        }
    }
    if (ledState == HIGH){
        write_D1();
        print_E();
        delay(1);
        write_D2();
        print_n();
        delay(1);
        write_D3();
        print_d();
        delay(1);

     }
     print_off(); //per spegnere tutto altrimenti rimane D4 acceso
     if(buttonState2 < 500 || buttonState1 < 500){
         fine = false;
         myLoop = false;
         delay(500);
          
    }
  }
  
}

void check(){
      if(!flag){
        if(!mode){ // MODALITA 11 PUNTI

        /*     AGGIORNO CHI BATTE    */
            
            Serial.println(nextChange);
            Serial.println(cont);
            Serial.println(" --------- ");
            if(cont == nextChange){
              switchBattitore();
              nextChange +=2;
            }
        /*     CONTROLLO SE ENTRO NEL SPAREMATCH    */
            if (((player1D1 == final11_1) && (player2D3 == final11_1)) && ((player1D2 == final11_2 - 1) && (player2D4 == final11_2 -1))){  // se p1 = 11 e p2 = 11 matchfinale = final11+1
                
                 if(final11_2 < 9){
                    final11_2 ++;
                 }else{
                  final11_2 = 0;
                  final11_1 ++;
                 }
                 check();
              }
                /*     CHECK PLAYERS WIN   */
            if(player1D1 == final11_1 && player1D2 == final11_2){ //chek player 1 if has win 
              winner(true);
            }

            if(player2D3 == final11_1 && player2D4 == final11_2){ //chek player 2 if has win 
              winner(true);
            }
            cont ++;
            
        }else{ // MODALITA 21 PUNTI
            Serial.println(nextChange);
        /*     AGGIORNO CHI BATTE    */
            if(cont == nextChange){
              switchBattitore();
              nextChange += 5;
            }
        /*     CONTROLLO SE ENTRO NEL SPAREMATCH    */
            if (((player1D1 == final21_1) && (player2D3 == final21_1)) && ((player1D2 == final21_2 - 1) && (player2D4 == final21_2 -1))){  // se p1 = 11 e p2 = 11 matchfinale = final11+1
                
                 if(final11_2 < 9){
                    final11_2 ++;
                 }else{
                  final11_2 = 0;
                  final11_1 ++;
                 }
                 check();
              }
                /*     CHECK PLAYERS WIN   */
            if(player1D1 == final21_1 && player1D2 == final21_2){ //chek player 1 if has win 
              winner(true);
            }

            if(player2D3 == final21_1 && player2D4 == final21_2){ //chek player 2 if has win 
              winner(true);
            }else{
              
            }
            cont ++;
        }
      }
}


void switchBattitore(){
        if(battitore){
        battitore = false;
        }else{
        battitore = true;
        }
}


void firstPoint(){ 
  if(player1D2 > player2D4){
    battitore = true;
    player1D2 --;
  }else if(player1D2 < player2D4){
    battitore = false;
    player2D4 --;
  }
  
}

void Display( int points){
  switch(points){
    case 0: print_0(); break;
    case 1: print_1(); break;
    case 2: print_2(); break;
    case 3: print_3(); break;
    case 4: print_4(); break;
    case 5: print_5(); break;
    case 6: print_6(); break;
    case 7: print_7(); break;
    case 8: print_8(); break;
    case 9: print_9(); break;
  }
}



/* THE FUNCTIONS */


void selectMode(){
  bool selected = false;
  bool setMode = true; // usato come flag per settare: mode 
  int myTime = 0;
  int myPreviousTime = 0;
  
  while(!selected){
  buttonState1 = analogRead(button1);
  buttonState2 = analogRead(button2);
 // Serial.println(buttonState1);
  
    if (!mode){
      write_D1();
      print_P();
      delay(1);
      write_D2();
      print_o();
      delay(1);
      write_D3();
      print_1();
      delay(1);
      write_D4();
      print_1();
      delay(1);
      nextChange = 0;
    }
    
    if (mode){
      write_D1();
      print_P();
      delay(1);
      write_D2();
      print_o();
      delay(1);
      write_D3();
      print_2();
      delay(1);
      write_D4();
      print_1();
      delay(1);
      nextChange = 4; // per far funzionare il cambio a 5 battute
    }
    
    print_off();
    if(buttonState1 < 500){  //delay per far si che i giocatori premano nello stesso momento
      digitalWrite(pinG, HIGH);
      delay(1200);
      buttonState2 = analogRead(button2);
      
    }
    if(buttonState2 < 500){
      digitalWrite(pinG, HIGH);
      delay(1200);
      buttonState1 = analogRead(button1);
    }
    
    if(buttonState1 < 500 && buttonState2 < 500){  
      selected = true;
      write_D1();
      print_P();
      delay(400);
      write_D2();
      print_I();
      delay(400);
      write_D3();
      print_a();
      delay(400);
      write_D4();
      print_y();
      delay(400);
      setMode= false;
    }
    
    
    if ((buttonState1 < 500 || buttonState2 < 500) && setMode){

      print_off();
      setMode = false;
      
      if(mode){
        mode = false;
      }else{
        mode = true;
        }
      digitalWrite(pinG, HIGH);
      delay(600);
    }
    setMode = true;
  } //end while
}



// the functions for selecting the common pin to turn on
void write_D1()
{
  digitalWrite(D1, LOW);
  digitalWrite(D2, HIGH);
  digitalWrite(D3, HIGH);
  digitalWrite(D4, HIGH); 
}

void write_D2()
{
  digitalWrite(D1, HIGH);
  digitalWrite(D2, LOW);
  digitalWrite(D3, HIGH);
  digitalWrite(D4, HIGH); 
}

void write_D3()
{
  digitalWrite(D1, HIGH);
  digitalWrite(D2, HIGH);
  digitalWrite(D3, LOW);
  digitalWrite(D4, HIGH); 
}

void write_D4()
{
  digitalWrite(D1, HIGH);
  digitalWrite(D2, HIGH);
  digitalWrite(D3, HIGH);
  digitalWrite(D4, LOW); 
}

// the functions for writing characters to the display
void print_0() // writing 0
{
  digitalWrite(pinA, HIGH);   
  digitalWrite(pinB, HIGH);   
  digitalWrite(pinC, HIGH);   
  digitalWrite(pinD, HIGH);   
  digitalWrite(pinE, HIGH);   
  digitalWrite(pinF, HIGH);   
  digitalWrite(pinG, LOW);
  digitalWrite(pinDP, LOW); // the decimal point is always off by default
}

void print_1() // writing 1
{
  digitalWrite(pinA, LOW);   
  digitalWrite(pinB, HIGH);   
  digitalWrite(pinC, HIGH);   
  digitalWrite(pinD, LOW);   
  digitalWrite(pinE, LOW);   
  digitalWrite(pinF, LOW);   
  digitalWrite(pinG, LOW);
  digitalWrite(pinDP, LOW);
}

void print_2() // writing 2
{
  digitalWrite(pinA, HIGH);   
  digitalWrite(pinB, HIGH);   
  digitalWrite(pinC, LOW);   
  digitalWrite(pinD, HIGH);   
  digitalWrite(pinE, HIGH);   
  digitalWrite(pinF, LOW);   
  digitalWrite(pinG, HIGH);
  digitalWrite(pinDP, LOW);
}

void print_3() // writing 3
{
  digitalWrite(pinA, HIGH);   
  digitalWrite(pinB, HIGH);   
  digitalWrite(pinC, HIGH);   
  digitalWrite(pinD, HIGH);   
  digitalWrite(pinE, LOW);   
  digitalWrite(pinF, LOW);   
  digitalWrite(pinG, HIGH);
  digitalWrite(pinDP, LOW);
}

void print_4() // writing 4
{
  digitalWrite(pinA, LOW);   
  digitalWrite(pinB, HIGH);   
  digitalWrite(pinC, HIGH);   
  digitalWrite(pinD, LOW);   
  digitalWrite(pinE, LOW);   
  digitalWrite(pinF, HIGH);   
  digitalWrite(pinG, HIGH);
  digitalWrite(pinDP, LOW);
}

void print_5() // writing 5
{
  digitalWrite(pinA, HIGH);   
  digitalWrite(pinB, LOW);   
  digitalWrite(pinC, HIGH);   
  digitalWrite(pinD, HIGH);   
  digitalWrite(pinE, LOW);   
  digitalWrite(pinF, HIGH);   
  digitalWrite(pinG, HIGH);
  digitalWrite(pinDP, LOW);
}

void print_6() // writing 6
{
  digitalWrite(pinA, HIGH);   
  digitalWrite(pinB, LOW);   
  digitalWrite(pinC, HIGH);   
  digitalWrite(pinD, HIGH);   
  digitalWrite(pinE, HIGH);   
  digitalWrite(pinF, HIGH);   
  digitalWrite(pinG, HIGH);
  digitalWrite(pinDP, LOW);
}

void print_7() // writing 7
{
  digitalWrite(pinA, HIGH);   
  digitalWrite(pinB, HIGH);   
  digitalWrite(pinC, HIGH);   
  digitalWrite(pinD, LOW);   
  digitalWrite(pinE, LOW);   
  digitalWrite(pinF, LOW);   
  digitalWrite(pinG, LOW);
  digitalWrite(pinDP, LOW);
}

void print_8() // writing 8
{
  digitalWrite(pinA, HIGH);   
  digitalWrite(pinB, HIGH);   
  digitalWrite(pinC, HIGH);   
  digitalWrite(pinD, HIGH);   
  digitalWrite(pinE, HIGH);   
  digitalWrite(pinF, HIGH);   
  digitalWrite(pinG, HIGH);
  digitalWrite(pinDP, LOW);
}

void print_9() // writing 9
{
  digitalWrite(pinA, HIGH);   
  digitalWrite(pinB, HIGH);   
  digitalWrite(pinC, HIGH);   
  digitalWrite(pinD, HIGH);   
  digitalWrite(pinE, LOW);   
  digitalWrite(pinF, HIGH);   
  digitalWrite(pinG, HIGH);
  digitalWrite(pinDP, LOW);
}

void print_A() // writing A
{
  digitalWrite(pinA, HIGH);   
  digitalWrite(pinB, HIGH);   
  digitalWrite(pinC, HIGH);   
  digitalWrite(pinD, LOW);   
  digitalWrite(pinE, HIGH);   
  digitalWrite(pinF, HIGH);   
  digitalWrite(pinG, HIGH);
  digitalWrite(pinDP, LOW);
}
void print_a() // writing a
{
  digitalWrite(pinA, HIGH);   
  digitalWrite(pinB, HIGH);   
  digitalWrite(pinC, HIGH);   
  digitalWrite(pinD, HIGH);   
  digitalWrite(pinE, HIGH);   
  digitalWrite(pinF, LOW);   
  digitalWrite(pinG, HIGH);
  digitalWrite(pinDP, LOW);
}

void print_b() // writing b
{
  digitalWrite(pinA, LOW);   
  digitalWrite(pinB, LOW);   
  digitalWrite(pinC, HIGH);   
  digitalWrite(pinD, HIGH);   
  digitalWrite(pinE, HIGH);   
  digitalWrite(pinF, HIGH);   
  digitalWrite(pinG, HIGH);
  digitalWrite(pinDP, LOW);
}

void print_C() // writing C
{
  digitalWrite(pinA, HIGH);   
  digitalWrite(pinB, LOW);   
  digitalWrite(pinC, LOW);   
  digitalWrite(pinD, HIGH);   
  digitalWrite(pinE, HIGH);   
  digitalWrite(pinF, HIGH);   
  digitalWrite(pinG, LOW);
  digitalWrite(pinDP, LOW);
}

void print_d() // writing d
{
  digitalWrite(pinA, LOW);   
  digitalWrite(pinB, HIGH);   
  digitalWrite(pinC, HIGH);   
  digitalWrite(pinD, HIGH);   
  digitalWrite(pinE, HIGH);   
  digitalWrite(pinF, LOW);   
  digitalWrite(pinG, HIGH);
  digitalWrite(pinDP, LOW);
}

void print_E() // writing E
{
  digitalWrite(pinA, HIGH);   
  digitalWrite(pinB, LOW);   
  digitalWrite(pinC, LOW);   
  digitalWrite(pinD, HIGH);   
  digitalWrite(pinE, HIGH);   
  digitalWrite(pinF, HIGH);   
  digitalWrite(pinG, HIGH);
  digitalWrite(pinDP, LOW);
}

void print_F() // writing F
{
  digitalWrite(pinA, HIGH);   
  digitalWrite(pinB, LOW);   
  digitalWrite(pinC, LOW);   
  digitalWrite(pinD, LOW);   
  digitalWrite(pinE, HIGH);   
  digitalWrite(pinF, HIGH);   
  digitalWrite(pinG, HIGH);
  digitalWrite(pinDP, LOW);
}

void print_H() // writing H
{
  digitalWrite(pinA, LOW);   
  digitalWrite(pinB, HIGH);   
  digitalWrite(pinC, HIGH);   
  digitalWrite(pinD, LOW);   
  digitalWrite(pinE, HIGH);   
  digitalWrite(pinF, HIGH);   
  digitalWrite(pinG, HIGH);
  digitalWrite(pinDP, LOW);
}

void print_h() // writing h
{
  digitalWrite(pinA, LOW);   
  digitalWrite(pinB, LOW);   
  digitalWrite(pinC, HIGH);   
  digitalWrite(pinD, LOW);   
  digitalWrite(pinE, HIGH);   
  digitalWrite(pinF, HIGH);   
  digitalWrite(pinG, HIGH);
  digitalWrite(pinDP, LOW);
}

void print_I() // writing i
{
  digitalWrite(pinA, LOW);   
  digitalWrite(pinB, LOW);   
  digitalWrite(pinC, LOW);   
  digitalWrite(pinD, LOW);   
  digitalWrite(pinE, HIGH);   
  digitalWrite(pinF, HIGH);   
  digitalWrite(pinG, LOW);
  digitalWrite(pinDP, LOW);
}


void print_J() // writing J
{
  digitalWrite(pinA, LOW);   
  digitalWrite(pinB, HIGH);   
  digitalWrite(pinC, HIGH);   
  digitalWrite(pinD, HIGH);   
  digitalWrite(pinE, LOW);   
  digitalWrite(pinF, LOW);   
  digitalWrite(pinG, LOW);
  digitalWrite(pinDP, LOW);
}

void print_n() // writing n
{
  digitalWrite(pinA, LOW);      
  digitalWrite(pinB, LOW);   
  digitalWrite(pinC, HIGH);   
  digitalWrite(pinD, LOW);   
  digitalWrite(pinE, HIGH);   
  digitalWrite(pinF, LOW);   
  digitalWrite(pinG, HIGH);
  digitalWrite(pinDP, LOW);
}

void print_N() // writing n
{
  digitalWrite(pinA, HIGH);   
  digitalWrite(pinB, HIGH);   
  digitalWrite(pinC, HIGH);   
  digitalWrite(pinD, LOW);   
  digitalWrite(pinE, HIGH);   
  digitalWrite(pinF, HIGH);   
  digitalWrite(pinG, LOW);
  digitalWrite(pinDP, LOW);   
}

void print_O() // writing O
{
  digitalWrite(pinA, HIGH);   
  digitalWrite(pinB, HIGH);   
  digitalWrite(pinC, HIGH);   
  digitalWrite(pinD, HIGH);   
  digitalWrite(pinE, HIGH);   
  digitalWrite(pinF, HIGH);   
  digitalWrite(pinG, LOW);
  digitalWrite(pinDP, LOW);
}

void print_o() // writing o
{
  digitalWrite(pinA, LOW);   
  digitalWrite(pinB, LOW);   
  digitalWrite(pinC, HIGH);   
  digitalWrite(pinD, HIGH);   
  digitalWrite(pinE, HIGH);   
  digitalWrite(pinF, LOW);   
  digitalWrite(pinG, HIGH);
  digitalWrite(pinDP, LOW);
}

void print_P() // writing P
{
  digitalWrite(pinA, HIGH);   
  digitalWrite(pinB, HIGH);   
  digitalWrite(pinC, LOW);   
  digitalWrite(pinD, LOW);   
  digitalWrite(pinE, HIGH);   
  digitalWrite(pinF, HIGH);   
  digitalWrite(pinG, HIGH);
  digitalWrite(pinDP, LOW);
}

void print_r() // writing r
{
  digitalWrite(pinA, LOW);   
  digitalWrite(pinB, LOW);   
  digitalWrite(pinC, LOW);   
  digitalWrite(pinD, LOW);   
  digitalWrite(pinE, HIGH);   
  digitalWrite(pinF, LOW);   
  digitalWrite(pinG, HIGH);
  digitalWrite(pinDP, LOW);
}

void print_t() // writing t
{
  digitalWrite(pinA, LOW);   
  digitalWrite(pinB, LOW);   
  digitalWrite(pinC, LOW);   
  digitalWrite(pinD, HIGH);   
  digitalWrite(pinE, HIGH);   
  digitalWrite(pinF, HIGH);   
  digitalWrite(pinG, HIGH);
  digitalWrite(pinDP, LOW);
}
void print_S()//writing S
{
  digitalWrite(pinA, HIGH);   
  digitalWrite(pinB, LOW);   
  digitalWrite(pinC, HIGH);   
  digitalWrite(pinD, HIGH);   
  digitalWrite(pinE, LOW);   
  digitalWrite(pinF, HIGH);   
  digitalWrite(pinG, HIGH);
  digitalWrite(pinDP, LOW);
}


void print_u() // writing u
{
  digitalWrite(pinA, LOW);   
  digitalWrite(pinB, LOW);   
  digitalWrite(pinC, HIGH);   
  digitalWrite(pinD, HIGH);   
  digitalWrite(pinE, HIGH);   
  digitalWrite(pinF, LOW);   
  digitalWrite(pinG, LOW);
  digitalWrite(pinDP, LOW);
}

void print_y() // writing y
{
  digitalWrite(pinA, LOW);   
  digitalWrite(pinB, HIGH);   
  digitalWrite(pinC, HIGH);   
  digitalWrite(pinD, HIGH);   
  digitalWrite(pinE, LOW);   
  digitalWrite(pinF, HIGH);   
  digitalWrite(pinG, HIGH);
  digitalWrite(pinDP, LOW);
}


void print_decimal() // writing the decimal point
{
  digitalWrite(pinDP, HIGH);
}

void print_off() // turning all 7 segments plus the decimal point off
{
  digitalWrite(pinA, LOW);   
  digitalWrite(pinB, LOW);   
  digitalWrite(pinC, LOW);   
  digitalWrite(pinD, LOW);   
  digitalWrite(pinE, LOW);   
  digitalWrite(pinF, LOW);   
  digitalWrite(pinG, LOW);
  digitalWrite(pinDP, LOW);
}
