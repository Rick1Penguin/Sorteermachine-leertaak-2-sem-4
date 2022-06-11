#include <Servo.h>
Servo myservoL;
Servo myservoR;
int PotwaardeL;
int PotwaardeR;
int HoekL;
int HoekR;
int knop = 53;

void setup() {
  pinMode(A0,INPUT);
  pinMode(A1,INPUT);
  pinMode(53,INPUT);
  myservoR.attach(10);
  myservoL.attach(8);
Serial.begin(9600);

}

void loop() {
 PotwaardeL = analogRead(A1);
 PotwaardeR = analogRead(A0);
 Serial.print(" Pot meter waarde L:  ");
 Serial.print(PotwaardeL);
 HoekL = map(PotwaardeL, 0, 1023, 0, 180);
 Serial.print(" Hoek L:  ");
 Serial.println(HoekL);
 
 Serial.print(" Pot meter waarde R:  ");
 Serial.print(PotwaardeR);
 HoekR = map(PotwaardeR, 0, 1023, 0, 180);
 Serial.print(" Hoek R:  ");
 Serial.println(HoekR);
 
  
 delay(200);
 if (digitalRead(knop)==1){
  
  myservoR.write(HoekR);
  myservoL.write(HoekL);
 }
 }
