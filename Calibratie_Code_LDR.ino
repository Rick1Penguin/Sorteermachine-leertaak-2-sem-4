
const int IRPinBand = 43; //de echopin van de ultrasoon voor de bakjes orientatie
const int bandMotorPin = 31; //de pin die aangesloten staat op de MOSFET van de bandmotor
const int LDRPin = A15; //de pin die is aangesloten op de LDR
const int comPinIn = 22; //de pin die wordt gebruikt om te communiceren naar de andere arduino (in)

// Alle globale variabele worden hier gedefineerd:
int LDRWaarde;
bool IRBandStatus;


void setup() {
  //De setup:
  Serial.begin(9600); // Serial Communication is starting with 9600 of baudrate speed

  pinMode(IRPinBand, INPUT); 
  pinMode(bandMotorPin, OUTPUT);
  pinMode(LDRPin, INPUT);
  pinMode(comPinIn, INPUT);

  //Alle actuatoren worden naar de beginpositie gebracht:
  digitalWrite(bandMotorPin, LOW);
  Serial.println("Setup Complete.");
}

void loop() {
  //de main code:
  //Eerst wordt er gekeken of er al een bakje op de band staat en of deze vol is of niet
  IRBandStatus = digitalRead(IRPinBand);
  //if statements voor kijken of er een bakje voor de ldr staat en welke dit is:

  Serial.print("IRStatus:   ");
  Serial.println(IRBandStatus);
  digitalWrite(bandMotorPin, HIGH);
  
  while(IRBandStatus == 0){
  Serial.println("Er staat een bakje:");
  delay(100);
  digitalWrite(bandMotorPin, LOW);
  delay(1000);
  LDRWaarde = analogRead(LDRPin); //LDR wordt uitgelezen
    Serial.print("LDRWaarde");
    Serial.println(LDRWaarde);
    IRBandStatus = digitalRead(IRPinBand);
    if(digitalRead(comPinIn)==1){
  digitalWrite(bandMotorPin, HIGH);
  delay(1000);
    }
  }


    
}
