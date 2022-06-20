//Deze code gaat samen met de code voor het knikkersysteem, de 2 arduinos communiceren met elkaar
//door 3 pinnen die een Bool signaal krijgen of geven
//De codes zijn opgedeeld in 2 arduinos zodat het als 2 losse delen beschouwd kan worden
//dit is makkelijker met debuggen en problemen oplossen
//De noodstop werkt tijdens de delays

#include <Servo.h> //Servo library toegevoegd

//Alle servo van dit systeem worden gedeclareerd:
Servo draaiServo;
Servo klepServo1; // linker servo vanaf de trechter
Servo klepServo2; // rechter servo vanaf de trechter

// Alle pinnen worden in variabelen gezet:
const uint8_t echoPin = 53; //de echopin van de ultrasoon voor de bakjes orientatie
const uint8_t trigPin = 51; //de trigpin van de ultrasoon voor de bakjes orientatie
const uint8_t IRPinBand = 43; //de pin van de ir onderbrekingssensor
const uint8_t bandMotorPin = 31; //de pin die aangesloten staat op de MOSFET van de bandmotor
const uint8_t trechterMotorPin = 35; //de pin die aangesloten staat op de MOSFET van de trechtermotor
const uint8_t draaiServoPin = 4; //de pin die is aangesloten op de servo van de draaischijf voor het doseren
const uint8_t klepServo1Pin = 6; //de pin die is aangesloten op de linker klep
const uint8_t klepServo2Pin = 8; //de pin die is aangesloten op de rechter klep
const uint8_t LDRPin = A15; //de pin die is aangesloten op de LDR
const uint8_t comPinOut1 = 29; //de pin die wordt gebruikt om te communiceren naar de andere arduino (out)
const uint8_t comPinOut2 = 27; //de pin die wordt gebruikt om te communiceren naar de andere arduino (out)
const uint8_t comPinIn = 22; //de pin die wordt gebruikt om te communiceren naar de andere arduino (in)
const uint8_t noodstopPin = 2; //de noodstoppin

// Alle globale variabele worden hier gedefineerd:
float afstand; // variable for the distance measurement of the ultrasonic sensor
int LDRWaarde;
unsigned long tijdSindsBakje = millis();
volatile bool noodstopPlaatsgevonden = 0;
bool trechterMotorStatus;
bool bandMotorStatus;
bool IRBandStatus;

// de wachttijd voot het transporteren van een bakje naar het knikkersysteem:
const int wachttijdBakje = 8000; // 8 seconden

//Callibratie Ultrasoon sensor:
const float ultrasoon_BakjeLaag = 6.5; // de lage afstand in cm voor wanneer een bakje met de open kant naar de sensor ligt
const uint8_t ultrasoon_BakjeHoog = 10; // de hoge afstand in cm voor wanneer een bakje met de open kant naar de sensor ligt

//Callibratie LDR sensor:
const int LDR_bakjeDoorzichtigLaag = 70;
const int LDR_bakjeDoorzichtigHoog = 150;
const int LDR_bakjePVCLaag = 5;
const int LDR_bakjePVCHoog = 39;
const int LDR_bakjeAluminiumLaag = 40;
const int LDR_bakjeAluminiumHoog = 60;

void setup() {
  //De setup:
  Serial.begin(9600); // Serial Communication is starting with 9600 of baudrate speed

  //De interupt voor de noodstop:
  attachInterrupt (digitalPinToInterrupt (noodstopPin), noodstop, CHANGE); 
  
  //Alle pinmodes worden hier gedefineerd:
  pinMode(trigPin, OUTPUT); // Sets the Pin as an OUTPUT
  pinMode(echoPin, INPUT); // Sets the Pin as an INPUT
  pinMode(IRPinBand, INPUT); 
  pinMode(bandMotorPin, OUTPUT);
  pinMode(trechterMotorPin, OUTPUT);
  pinMode(draaiServoPin, OUTPUT);
  pinMode(klepServo1Pin, OUTPUT);
  pinMode(klepServo2Pin, OUTPUT);
  pinMode(LDRPin, INPUT);
  pinMode(comPinOut1, OUTPUT);
  pinMode(comPinOut2, OUTPUT);
  pinMode(comPinIn, INPUT);

  //Alle Servos worden verbonden met de juiste pin:
  draaiServo.attach(draaiServoPin);
  klepServo1.attach(klepServo1Pin);
  klepServo2.attach(klepServo2Pin);

  //Alle actuatoren worden naar de beginpositie gebracht:
  digitalWrite(trechterMotorPin, LOW);
  digitalWrite(bandMotorPin, LOW);
  digitalWrite(comPinOut1, LOW);
  digitalWrite(comPinOut2, LOW);
  draaiServo.write(160); //dit is de posietie waarbij het gat bij de trechter zit
  klepServo1.write(145); //dit is de positie waarbij de klepjes naar beneden staan
  klepServo2.write(20); //dit is de positie waarbij de klepjes naar beneden staan
  
  Serial.println("Setup Complete.");
}

void loop() {
  //de main code:
  //Eerst wordt er gekeken of er een bakje voor het knikkergat staat:
  IRBandStatus = digitalRead(IRPinBand); // de IR onderbrekingssensor
  if(IRBandStatus == 0){
    // Als de lichtstroom onderbroken is wordt er gekeken welk bakje het is:
    Serial.println("Er staat een bakje, namelijk:");
    delay(100);
    digitalWrite(bandMotorPin, LOW); //Motor wordt stilgezet op de juiste plek
    delay(1000); // delay om een constante meting te krijgen va de LDR
    LDRWaarde = analogRead(LDRPin); //LDR wordt uitgelezen
  if(LDRWaarde >= LDR_bakjeDoorzichtigLaag && LDRWaarde <= LDR_bakjeDoorzichtigHoog ){
    digitalWrite(comPinOut1, HIGH); //communiceren dat er een Doorzichtig bakje staat
    digitalWrite(comPinOut2, HIGH); //communiceren dat er een Doorzichtig bakje staat
    Serial.print(LDRWaarde);
    Serial.println("Doorzichtig bakje");
    afvoerBakje(); //doorgestuurd naar de functie waar gekeken wordt of het bakje vol is en afgevoerd wordt
    }
  else if(LDRWaarde >= LDR_bakjePVCLaag && LDRWaarde <= LDR_bakjePVCHoog ){
    digitalWrite(comPinOut1, HIGH); //communiceren dat er een PVC bakje staat
    digitalWrite(comPinOut2, LOW); //communiceren dat er een PVC bakje staat
    Serial.print(LDRWaarde);
    Serial.println("PVC bakje");
    afvoerBakje(); //doorgestuurd naar de functie waar gekeken wordt of het bakje vol is en afgevoerd wordt
    }  
  else if(LDRWaarde >= LDR_bakjeAluminiumLaag && LDRWaarde <= LDR_bakjeAluminiumHoog ){
    digitalWrite(comPinOut1, LOW); //communiceren dat er een Aluminium bakje staat
    digitalWrite(comPinOut2, HIGH); //communiceren dat er een Aluminium bakje staat
    Serial.print(LDRWaarde);
    Serial.println("Aluminium bakje");
    afvoerBakje(); //doorgestuurd naar de functie waar gekeken wordt of het bakje vol is en afgevoerd wordt
    }
    //Als geen van de bakjes voldeed wordt er teruggegaan en opnieuw gemeten
  }
  else if (IRBandStatus==1){
    //Als de lichtstroom niet onderbroken is wordt er doorgegeven dat er geen bakje staat
    digitalWrite(comPinOut1, LOW); //communiceren dat er geen bakje staat
    digitalWrite(comPinOut2, LOW); //communiceren dat er geen bakje staat
    
    //checken of de tijd sinds het bakjes programma langer is geweest dan de wachttijd
    if(millis()- tijdSindsBakje >= wachttijdBakje){
      digitalWrite(bandMotorPin, LOW);
      bakjesCode(); // een bakje op de rolband leggen en de tijdSindsBakje resetten
      }
    else{
      digitalWrite(bandMotorPin, HIGH); //motor van de band aanzetten omdat het bakje niet direct voor het gat staat
    }
    }
}

void noodstop(){
  //de noodstop functie die vast staat aan een interrupt pin:
  bool noodstopStatus = digitalRead(noodstopPin);
  if(noodstopStatus == HIGH){
    trechterMotorStatus = digitalRead(trechterMotorPin); //status van motoren registreren
    bandMotorStatus = digitalRead(bandMotorPin);
    digitalWrite(trechterMotorPin, LOW); //motoren uitzetten
    digitalWrite(bandMotorPin, LOW);
    noodstopPlaatsgevonden = 1; //variabele om te weten of er in het programma eventueel metingen opnieuw gedaan moeten worden
    Serial.println("Noodstop geactiveerd");
  }
  while(noodstopStatus == HIGH){
    noodstopStatus = digitalRead(noodstopPin); //loop om de arduino te freezen
  }
  if(noodstopPlaatsgevonden == 1){
    //alle voorheen geactiveerde actuatoren weer aanzetten
    digitalWrite(trechterMotorPin, trechterMotorStatus);
    digitalWrite(bandMotorPin, bandMotorStatus);
    Serial.println("Noodstop gedeactiveerd");
  }
}

void bakjesCode(){
  //de functie van het plaatsen van een bakje op de band:
  digitalWrite(trechterMotorPin, HIGH); //vibratie motor op trechter wordt aangezet
  klepServo1.write(50); //dit is de positie waarbij het bakje op de klepjes moet landen
  klepServo2.write(92); //dit is de positie waarbij het bakje op de klepjes moet landen
  delay(3000);//wachttijd voor trechter
  
  // doseerschijf  laten draaien:
  draaiServo.write(57); //gatpositie
  delay(1000);
  draaiServo.write(52); //iets verder
  delay(1000);
  draaiServo.write(57); //gatpositie
  delay(1000);
  draaiServo.write(160); //beginpositie
  delay(1500);

  digitalWrite(trechterMotorPin, LOW); //vibratie motor op trechter wordt uitgezet

  //Ultrasoon uitlezen en bepalen orientatie bakje
  do{
  ultrasoonOrientatie();
  }while(noodstopPlaatsgevonden == 1);
  
  Serial.print("afstand bakje: ");
  Serial.print(afstand);
  Serial.println(" cm");
  
  if (afstand >= ultrasoon_BakjeLaag && afstand <= ultrasoon_BakjeHoog) { 
    //bakje laten zakken met opening naar ultrasoon:
    klepServo1.write(145);
    klepServo2.write(92);
    delay(300);
    klepServo2.write(70);
    delay(300);
    klepServo2.write(60);
    delay(300);
    klepServo2.write(50);
    delay(300);
    klepServo2.write(40);
    delay(300);
    klepServo2.write(30);
    delay(200);
    klepServo2.write(20);
  }
  else { 
    //servo laten zakken met dichte kant naar ultrasoon:
    klepServo1.write(50);
    klepServo2.write(20);
    delay(300);
    klepServo1.write(60);
    delay(300);
    klepServo1.write(70);
    delay(300);
    klepServo1.write(80);
    delay(300);
    klepServo1.write(90);
    delay(300);
    klepServo1.write(100);
    delay(300);
    klepServo1.write(110);
    delay(300);
    klepServo1.write(121);
    delay(300);
    klepServo1.write(145);
  }
  delay(500); //een kleine delay zodat de band pas gaat rollen als het bakje daadwerkelijk om de band staat
  tijdSindsBakje = millis(); //omdat er net een bakje opgezet is moet dit geregistreerd worden
  Serial.println("Bakje op de band gelegd");
  }

void afvoerBakje(){
  bool bakjeVol;
  do {
   bakjeVol = digitalRead(comPinIn); //com met andere arduino
   IRBandStatus = digitalRead(IRPinBand); // de IR onderbrekingssensor
  } while (bakjeVol == LOW && IRBandStatus == LOW); //terwijl de andere arduino aangeeft dat het bakje leeg is en er een bakje voor staat blijven loopen
  digitalWrite(bandMotorPin, HIGH); //transportband aan
  digitalWrite(comPinOut1, LOW); //communiceren dat er geen bakje staat
  digitalWrite(comPinOut2, LOW); //communiceren dat er geen bakje staat
  delay(2500); //wachten tot bakje is afgevoerd
  digitalWrite(bandMotorPin, LOW); //transportband uit
  Serial.println("Bakje afgevoerd");
}

void ultrasoonOrientatie(){ 
  //de functie om de ultrasoon van de bakjes orientatie uit te lezen:
  noodstopPlaatsgevonden = 0; //reset de noodstopplaatsgevonden, zodat als er tijdens de meting een noodstop plaats vind de meting opnieuw gaat
  // Clears the trigPin condition
  digitalWrite(trigPin, LOW); 
  delayMicroseconds(2);
  // Sets the trigPin HIGH (ACTIVE) for 10 microseconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  float duration = pulseIn(echoPin, HIGH);
  // Calculating the distance in cm
  afstand = duration * 0.034 / 2; // Speed of sound wave divided by 2 (go and back)
  }
//wh24 groep 3
