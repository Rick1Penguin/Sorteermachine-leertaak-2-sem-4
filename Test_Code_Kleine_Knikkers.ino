
#include <Servo.h> //Servo library toegevoegd

Servo stopper_1K;

Servo stopper_2K;

Servo sorteerServoK; // de sorteerservo voor de kleine knikkers

const uint8_t stopper_1KPin = 8; //servo pin
const uint8_t stopper_2KPin = 7; //servo pin
const uint8_t sorteerServoKPin = 6; //servo pin

const uint8_t noodstopPin = 2; //Noodstop pin

const uint8_t knikkerKleinDetectiePin = 31; //kliksensor pin
const uint8_t knikkerKleinMetaalDetectiePin = 33; //stroommeter pin

volatile bool noodstopPlaatsgevonden = 0;

void setup() {
  //De setup:
  Serial.begin(9600); // Serial Communication is starting with 9600 of baudrate speed

  //De interupt voor de noodstop:
  attachInterrupt (digitalPinToInterrupt (noodstopPin), noodstop, CHANGE); 
  
  //Alle pinmodes worden hier gedefineerd:
  pinMode(stopper_1KPin, OUTPUT);
  pinMode(stopper_2KPin, OUTPUT);
  pinMode(sorteerServoKPin, OUTPUT);
  
  pinMode(knikkerKleinDetectiePin, INPUT);
  pinMode(knikkerKleinMetaalDetectiePin, INPUT);
  
  stopper_1K.attach(stopper_1KPin);
  stopper_2K.attach(stopper_2KPin);
  sorteerServoK.attach(sorteerServoKPin);
  stopper_1K.write(95);
  stopper_2K.write(96);
  sorteerServoK.write(102);
  Serial.println("Setup Complete.");

  
}

void loop() {

  knikker_Klein(); //meten kleine knikkers onder en sorteren
  delay(5000); //om de 5 seconden meet hij een knikker uit

}

//de functie voor het sorteren van de kleine knikkers:
void knikker_Klein() {
  //Poortje openen en sluiten, om de knikkers te doseren:
  delay(500);
  Serial.println("meten kleine knikkers");
  stopper_2K.write(96); //servo 2 sluiten
  delay(500);
  stopper_1K.write(120); //eerste poortje open
  delay(500);
  stopper_1K.write(95); //eerste poortje dicht
  delay(500);
  
  //checken of er een knikker in het poortje zit:
  bool knikkerInPoortje = digitalRead(knikkerKleinDetectiePin);
  bool sensor;
  if(knikkerInPoortje == HIGH){
    sorteerServoK.write(102);//standaard naar de galzen buffer sturen
    delay(200);
    stopper_2K.write(135); //servo 2 openen
    unsigned long Tijd1 = millis();
    unsigned long Tijd2;
    
    do{
      // loopje herhalen totdat een bepaalde tijd is bereikt
      sensor = digitalRead(knikkerKleinMetaalDetectiePin);
      Tijd2 = millis();
    }while(Tijd2-Tijd1 <= 4000 && sensor == LOW);
    
    if(sensor == LOW){
      //als de sensor laag is was het een glazen knikker
      Serial.println("Glazen knikker gesorteerd bij de kleine knikkers");
    }
    else if(sensor == HIGH){
      //als de sensor hoog is was het een metalen knikker
      sorteerServoK.write(46);//naar de metalen buffer sturen
      Serial.println("Metalen knikker gesorteerd bij de kleine knikkers");      
    }
  }
  else{
    Serial.println("Geen knikker gesorteerd bij de kleine knikkers");
  }

}

void noodstop(){
  //de noodstop functie die vast staat aan een interrupt pin:
  bool noodstopStatus = digitalRead(noodstopPin);
  if(noodstopStatus == HIGH){
    noodstopPlaatsgevonden = 1; //variabele om te weten of er in het programma eventueel metingen opnieuw gedaan moeten worden
    Serial.println("Noodstop geactiveerd");
  }
  while(noodstopStatus == HIGH){
    noodstopStatus = digitalRead(noodstopPin); //loop om de arduino te freezen
  }
  if(noodstopPlaatsgevonden == 1){
    //alle voorheen geactiveerde actuatoren weer aanzetten
    Serial.println("Noodstop gedeactiveerd");
  }
}
