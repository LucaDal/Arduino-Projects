#define in1 10
#define in2 11
#define fcS 13    //Finecorsa Sinistro ( posizione di reset iniziale )
#define fcD 12    //Finecorsa Destro
#define pin_sensore A0
#define IR 2
boolean stato_pulsante;

void setup() {
  digitalWrite(in1, HIGH) ;
  digitalWrite(in2, HIGH) ;
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(fcS, INPUT_PULLUP);
  pinMode(fcD, INPUT_PULLUP);
  pinMode(IR,INPUT);
  pinMode(pin_sensore, INPUT_PULLUP);  
}

void loop() {

  
  if (digitalRead(pin_sensore) == 0 ) { // attende lo start
    stato_pulsante = true;       //ricordo che il pulsante è stato premuto  
  }
  
  if(stato_pulsante == true){
      apertura();
  }
  
   chiusura();

}// fine loop


  void apertura(){
    while (digitalRead(fcD) != 0)  {
    digitalWrite(in1, LOW) ; // motore avanti
   }
    digitalWrite(in1, HIGH) ;// motore_stop  
    stato_pulsante = false;  //
    delay(5000);
  }
 

  void chiusura(){

    int vals = digitalRead(IR);
    if(vals == LOW){
      apertura();
    }  else{
           delay(100);
           while (digitalRead(fcS) != 0)  {
           digitalWrite(in2, LOW) ; // motore avanti
            }
           digitalWrite(in2, HIGH) ;// motore_stop  
      }
    }
   

  
  
