
const int relayPin = 14;          //the relay module S pin connected to digital pin 8

void setup()
{
  pinMode(relayPin, OUTPUT);    //initialize the relayPin as an output
}

void loop()
{
  digitalWrite(relayPin, HIGH); //drive relay closure conduction
  delay(1000);                  //wait for a second

  digitalWrite(relayPin, LOW);  //drive the relay is closed off
  delay(1000);                  //wait for a second
}
