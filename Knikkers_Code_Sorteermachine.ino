#include <stdio.h>
#include <Servo.h>
//servo's
Servo sorteerServoK;
Servo sorteerServoG;
Servo doseerServoK;
Servo doseerServoG;
Servo stopper_1K;
Servo stopper_1G;
Servo stopper_2K;
Servo stopper_2G;
const uint8_t Servo_1 = 22;
const uint8_t Servo_2 = 23;
const uint8_t Servo_3 = 24;
const uint8_t Servo_4 = 25;
const uint8_t Servo_5 = 26;
const uint8_t Servo_6 = 27;
const uint8_t Servo_7 = 28;
const uint8_t Servo_8 = 29;
const uint8_t noodstop = 22;
const uint8_t S1 = 30;
const uint8_t S2 = 31;
const uint8_t S3 = 32;
const uint8_t sensorOut = 33;
const uint8_t S0 = 44;
const int comPinOut = 10; //de pin die wordt gebruikt om te communiceren naar de andere arduino (out)
const int comPinIn = A0; //de pin die wordt gebruikt om te communiceren naar de andere arduino (in)
int RGB_G[3];
int RGB_K[3];
int RGB_Base[3];
int Stdev_BaseR;
int Stdev_BaseG;
int Stdev_BaseB;
int aluminium[4] = {1 , 0, 0, 2}; //Glas,Metaal,Plastic,Hout
int doorzichtig[4] = {0, 2, 1, 0}; //Glas,Metaal,Plastic,Hout
int PVC[4] = {1, 1, 1, 0}; //Glas,Metaal,Plastic,Hout
int glas = 0;
int metaal = 0;
int plastic = 0;
int hout = 0;
int bakType;
bool flag;
bool flag_2;
bool flag_3;
bool flag_4;
bool flag_5 = HIGH;
uint8_t glas_aantal;
uint8_t metaal_aantal;
uint8_t plastic_aantal;
uint8_t hout_aantal;
uint8_t ticker;

void setup() {
  // Inputs
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  pinMode(S0, OUTPUT);
  pinMode(sensorOut, INPUT);
  pinMode(noodstop, INPUT); //Servo's
  stopper_1G.attach(Servo_1);
  stopper_2G.attach(Servo_2);
  sorteerServoG.attach(Servo_3);
  doseerServoG.attach(Servo_4);
  stopper_1K.attach(Servo_5);
  stopper_2K.attach(Servo_6);
  sorteerServoK.attach(Servo_7);
  doseerServoK.attach(Servo_8);
  
  //aanzetten motoren //Begin serial comms
  Serial.begin(9600);
  digitalWrite(S0, HIGH);
  digitalWrite(S1, LOW);
  
} void opzet() { //extra opzet void voor kalibratie functies
  stopper_1G.write(70); //alle servo's dicht
  stopper_2G.write(93);
  stopper_1K.write(95);
  stopper_2K.write(96);
  flag_5 = LOW;
  delay(5000);
}
int knikker_lezen(int j) {
foutmeting:
  int Red;
  int Green;
  int Blue;
  int Bluegem;
  int Redgem;
  int Greengem;
  uint32_t Rtot = 0;
  uint32_t Gtot = 0;
  uint32_t Btot = 0;
  digitalWrite(S2, LOW);
  digitalWrite(S3, LOW);
  delay(200);
  for (int i = 0; i < 100; i++) { //meten 100 rode waardes voor gemiddelde
    Red = pulseIn(sensorOut, LOW);
    Rtot = Red + Rtot;
  }
  Redgem = Rtot / 100; //gemiddelde berekenen
  digitalWrite(S2, HIGH);
  digitalWrite(S3, HIGH);
  delay(200);
  for (int i = 0; i < 100; i++) { //meten 100 groene waardes voor gemiddelde
    Green = pulseIn(sensorOut, LOW);
    Gtot = Green + Gtot;
    //Serial.println(Green);
    //Serial.println(Gtot);
  }
  Greengem = Gtot / 100; //gemiddelde berekenen digitalWrite(S2, LOW);
  digitalWrite(S3, HIGH);
  delay(200);
  for (int i = 0; i < 100; i++) { //meten 100 blauwe waardes voor gemiddelde
    Blue = pulseIn(sensorOut, LOW);
    Btot = Blue + Btot;
  }
  Bluegem = Btot / 100; //gemiddelde berekenen Serial.print("Rood: ");
  Serial.print("Rode: ");
  Serial.print(Redgem);
  Serial.print(" Groen: ");
  Serial.print(Greengem);
  Serial.print(" Blauw: ");
  Serial.println(Bluegem);
  switch (j) {
    case 0:
      if (Redgem > 300 && Redgem < 340 && Greengem > 340 && Greengem < 380) { //checken hout knikker
        hout++;
        sorteerServoG.write(0);
        stopper_2G.write(175);
        delay(500);
        stopper_2G.write(93);
        Serial.println("Hout");
        return 0;
      }
      else if (Redgem > 210 && Redgem < 260 && Greengem > 280 && Greengem < 340) { //checken plastic knikker
        plastic++;
        stopper_2G.write(175);
        sorteerServoG.write(69);
        delay(500);
        stopper_2G.write(93);
        Serial.println("Plastic");
        return 1;
      }
      else if (Redgem > 270 && Redgem < 420 && Greengem > 280 && Greengem < 480) {
        Serial.println("Geen knikker");
      }
      else {
        Serial.println("Foutmeting"); //wh24 groep 3
        goto foutmeting;
      }
      break;
    case 1:
      if (Redgem >= 260 && Redgem < 300 && Bluegem > 360 && Bluegem < 390) { //checken metaal knikker
        metaal++;
        sorteerServoK.write(46);
        delay(200);
        stopper_2K.write(135);
        delay(500);
        stopper_2K.write(96);
        Serial.println("metaal");
        return 0;
      }
      else if (Redgem > 260 && Redgem < 300 && Bluegem > 260 && Bluegem < 330) { //checken glas knikker
        glas++;
        sorteerServoK.write(102);
        delay(200);
        stopper_2K.write(135);
        delay(500);
        stopper_2K.write(96);
        Serial.println("glas");
        return 1;
      }
      else if (Redgem > 240 && Redgem < 260 && Greengem > 340 && Greengem < 380 && Bluegem > 350 ) {
        Serial.println("Geen knikker");
        return;
      }
      else {
        Serial.println("Foutmeting");
        goto foutmeting;
      }
      break;
  }
}
int knikker_boven() {
  Serial.println("meten bovenkant knikkers");
  delay(500);
  stopper_1G.write(122); //poortje open
  delay(145);
  stopper_1G.write(70); //poortje dicht
  delay(500);
  knikker_lezen(0);
  delay(2000);
}
int knikker_onder() {
  delay(500);
  Serial.println("meten onderkant knikkers");
  stopper_1K.write(120); //poortje open
  delay(105);
  stopper_1K.write(95); //poortje dicht
  delay(500);
  knikker_lezen(1);
  delay(200);
}
int check() {
  switch (bakType) { //glas,metaal,plastic.hout
    case 1:
      if (glas >= aluminium[0] && metaal >= aluminium[1] && plastic >= aluminium[2] && hout >= aluminium[3]) {
        return 1;
      }
      else {
        return 0;
      }
    case 2:
      if (glas >= doorzichtig[0] && metaal >= doorzichtig[1] && plastic >= doorzichtig[2] && hout >= doorzichtig[4]) {
        return 1;
      }
      else {
        return 0;
      }
    case 3:
      if (glas >= PVC[0] && metaal >= PVC[1] && plastic >= PVC[2] && hout >= PVC[3]) {
        return 1;
      }
      else {
        return 0;
      }
  }
}
int bakjes() {
 int soortBakje = analogRead(A0); //lezen wat de andere arduino stuurt
    if ( soortBakje >= 200 && soortBakje <= 400) {
      Serial.println(" Doorzichtig bakje !! ");
      bakType = 1;
      return 1; //terugsturen naar loop dat er een bakje is
    }
    if (soortBakje >= 500 && soortBakje <= 700) {
      Serial.println(" PVC bakje !! ");
      bakType = 2;
      return 1; //terugsturen naar loop dat er een bakje is
    }
    if ( soortBakje >= 800 && soortBakje <= 1000) {
      Serial.println(" aluminium bakje !! ");
      bakType = 3;
      return 1; //terugsturen naar loop dat er een bakje is
    } 
    if (soortBakje == 0){
      digitalWrite(comPinOut,LOW); //communiceren dat er een nieuw bakje nodig is
      return 0; //terugsturen naar loop dat er geen bakje is
  }
}

void doseer() {
  switch (bakType) { //verwerken welke knikkers er in moeten
    case 1:
      glas_aantal = aluminium[0];
      metaal_aantal = aluminium[1];
      plastic_aantal = aluminium[2];
      hout_aantal = aluminium[3];
      break;
    case 2:
      glas_aantal = doorzichtig[0];
      metaal_aantal = doorzichtig[1];
      plastic_aantal = doorzichtig[2];
      hout_aantal = doorzichtig[3];
      break;
    case 3:
      glas_aantal = PVC[0];
      metaal_aantal = PVC[1];
      plastic_aantal = PVC[2];
      hout_aantal = PVC[3];
      break;
  }
  ticker = 1;
knikkerCheck:
  if (glas_aantal = !0 && ticker == 1) {
    doseerServoK.write(0);
    delay(1000);
    doseerServoK.write(84);
    glas_aantal--;
    if (glas_aantal == 0) {
      ticker = 2;
    }
    goto knikkerCheck;
  }
  if (metaal_aantal = !0 && ticker == 2) {
    doseerServoK.write(180);
    delay(1000);
    doseerServoK.write(84);
    metaal_aantal--;
    if (metaal_aantal == 0)
      ticker = 3;
    goto knikkerCheck;
  }
  if (plastic_aantal = !0 && ticker == 3) {
    doseerServoG.write(0);
    delay(1000);
    doseerServoG.write(92);
    plastic_aantal--;
    if (plastic_aantal == 0)
      ticker = 4;
    goto knikkerCheck;
  }
  if (hout_aantal = !0 && ticker == 4) {
    doseerServoG.write(180);
    delay(1000);
    doseerServoG.write(92);
    hout_aantal--;
    if (hout_aantal == 0)
      ticker = 0;
    goto knikkerCheck;
  }
} void loop() {
  if (flag_5 == HIGH)
    opzet();
  knikker_boven(); //meten knikkers boven
  knikker_onder(); //meten knikkers onder
  if (flag == LOW) {
    flag = bakjes(); //meten bakjes gedeelte
  }
  if (check() == HIGH && flag == HIGH ) { //checken of er genoeg knikkers in de buffer zitten
    doseer(); //doseren knikkers
    digitalWrite(comPinOut,HIGH); //communiceren dat het bakje vol is
  }
}
