
#include <Servo.h> //Servo library toegevoegd

Servo stopper_1K;

Servo stopper_2K;

Servo sorteerServoK; // de sorteerservo voor de kleine knikkers

const uint8_t stopper_1KPin = 8; //servo pin
const uint8_t stopper_2KPin = 7; //servo pin
const uint8_t sorteerServoKPin = 6; //servo pin

const uint8_t noodstopPin = 2; //Noodstop pin

const uint8_t knikkerKleinFSRPin = A0; //kliksensor pin
const uint8_t knikkerKleinMetaalPin = 33; //stroommeter pin

volatile bool noodstopPlaatsgevonden = 0;

  //calibratie:
  int stopper_1Khoekopen = 105;
  int stopper_1Khoekdicht = 180;
  int stopper_2Khoekopen = 105;
  int stopper_2Khoekdicht = 180;
  int sorteerServoKhoeklinks = 102;
  int sorteerServoKhoekrechts = 55;

  int FSR_grensWaarde = 5;

void setup() {
  //De setup:
  Serial.begin(9600); // Serial Communication is starting with 9600 of baudrate speed

  //De interupt voor de noodstop:
  attachInterrupt (digitalPinToInterrupt (noodstopPin), noodstop, LOW); 
  
  //Alle pinmodes worden hier gedefineerd:
  pinMode(stopper_1KPin, OUTPUT);
  pinMode(stopper_2KPin, OUTPUT);
  pinMode(sorteerServoKPin, OUTPUT);
  
  pinMode(knikkerKleinFSRPin, INPUT);
  pinMode(knikkerKleinMetaalPin, INPUT);
  
  stopper_1K.attach(stopper_1KPin);
  stopper_2K.attach(stopper_2KPin);
  sorteerServoK.attach(sorteerServoKPin);
  
  stopper_1K.write(stopper_1Khoekdicht);
  stopper_2K.write(stopper_2Khoekdicht);
  sorteerServoK.write(sorteerServoKhoeklinks);

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
  stopper_2K.write(stopper_2Khoekdicht); //servo 2 sluiten
  delay(500);
  stopper_1K.write(stopper_2Khoekopen); //eerste poortje open
  delay(1000);
  stopper_1K.write(stopper_2Khoekdicht); //eerste poortje dicht
  delay(500);

  //het 2e poortje wordt geopend en er wordt gekeken welke knikker het is:
    bool sensor;
    int FSR;
    sorteerServoK.write(sorteerServoKhoeklinks);//standaard naar de galzen buffer sturen
    delay(200);
    stopper_2K.write(stopper_2Khoekopen); //servo 2 openen
    unsigned long Tijd1 = millis();
    unsigned long Tijd2;

    do{
      // loopje herhalen totdat een bepaalde tijd is bereikt
      sensor = digitalRead(knikkerKleinMetaalPin);
      FSR = analogRead(knikkerKleinFSRPin);
      Tijd2 = millis();
    }while(Tijd2-Tijd1 <= 3000 && sensor == LOW && FSR <= FSR_grensWaarde);

    if(sensor == LOW && FSR > FSR_grensWaarde){
      //als de sensor laag is was het een glazen knikker
      Serial.println("Glazen knikker gesorteerd bij de kleine knikkers");
    }
    else if(sensor == HIGH){
      //als de sensor hoog is was het een metalen knikker
      sorteerServoK.write(sorteerServoKhoekrechts);//naar de metalen buffer sturen
      Serial.println("Metalen knikker gesorteerd bij de kleine knikkers");      
    }
    else if(sensor == LOW && FSR <= FSR_grensWaarde){
      Serial.println("Geen knikker gesorteerd bij de kleine knikkers");
    }
  }



void noodstop(){
  //de noodstop functie die vast staat aan een interrupt pin:
  bool noodstopStatus = digitalRead(noodstopPin);
  if(noodstopStatus == LOW){
    noodstopPlaatsgevonden = 1; //variabele om te weten of er in het programma eventueel metingen opnieuw gedaan moeten worden
    Serial.println("Noodstop geactiveerd");
  }
  while(noodstopStatus == LOW){
    noodstopStatus = digitalRead(noodstopPin); //loop om de arduino te freezen
  }
  if(noodstopPlaatsgevonden == 1){
    //alle voorheen geactiveerde actuatoren weer aanzetten
    Serial.println("Noodstop gedeactiveerd");
  }
}
