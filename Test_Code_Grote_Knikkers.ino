#include <Servo.h>
Servo groteKnikkerServo;
#define S0 9
#define S1 13
#define S2 12
#define S3 11
#define sensorOut 10

// Calibration Values
// Get these from Calibration Sketch

int redMin = 28; // Red minimum value
int redMax = 178; // Red maximum value
int greenMin = 32; // Green minimum value
int greenMax = 199; // Green maximum value
int blueMin = 28; // Blue minimum value
int blueMax = 160; // Blue maximum value

// Variables for Color Pulse Width Measurements

int redPW = 0;
int greenPW = 0;
int bluePW = 0;

int redValue;
int greenValue;
int blueValue;

const int beginpositie = 18;
const int sensorpositie = 80;
const int eindpositie = 144;

void setup() {
  //alle servo's
  groteKnikkerServo.attach(3);
  //alle inputs
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  // alle outputs
  pinMode(sensorOut, INPUT);
  // Set Frequency scaling to 20%
  digitalWrite(S0, HIGH);
  digitalWrite(S1, LOW);
  //begin seriële monitor
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
 // begin rotatie
 groteKnikkerServo.write(beginpositie);
 delay(2000);
 groteKnikkerServo.write(sensorpositie);
 delay(2000);
 
  //sensoruitlezen

  // Read Red value
  redPW = getRedPW();
  // Map to value from 0-255
  redValue = map(redPW, redMin, redMax, 255, 0);
  // Delay to stabilize sensor

  // Read Green value
  greenPW = getGreenPW();
  // Map to value from 0-255
  greenValue = map(greenPW, greenMin, greenMax, 255, 0);
  // Delay to stabilize sensor


  // Read Blue value
  bluePW = getBluePW();
  // Map to value from 0-255
  blueValue = map(bluePW, blueMin, blueMax, 255, 0);
  // Delay to stabilize sensor

  // hout knikker
  if (redValue > 40 && redValue < 70 && greenValue > 30 && greenValue < 60 && blueValue > 30 && blueValue < 50) {
    Serial.println(" hout ");
    groteKnikkerServo.write(eindpositie);
    delay(2000);

  }
  // plastic knikker
  if (redValue > 100 && redValue < 130 && greenValue > 110 && greenValue < 140 && blueValue > 100 && blueValue < 130) {
    Serial.println(" plastic ");
    groteKnikkerServo.write(eindpositie);
    delay(2000);
  }
  else {
    Serial.println(" geen knikker ");
    groteKnikkerServo.write(beginpositie);
    delay(2000);
  }
  // Print output to Serial Monitor
  Serial.print("Red = ");
  Serial.print(redValue);
  Serial.print(" - Green = ");
  Serial.print(greenValue);
  Serial.print(" - Blue = ");
  Serial.println(blueValue);
}
// Function to read Red Pulse Widths
int getRedPW() {

  // Set sensor to read Red only
  digitalWrite(S2, LOW);
  digitalWrite(S3, LOW);
  // Define integer to represent Pulse Width
  int PW;
  // Read the output Pulse Width
  PW = pulseIn(sensorOut, LOW);
  // Return the value
  return PW;

}

// Function to read Green Pulse Widths
int getGreenPW() {

  // Set sensor to read Green only
  digitalWrite(S2, HIGH);
  digitalWrite(S3, HIGH);
  // Define integer to represent Pulse Width
  int PW;
  // Read the output Pulse Width
  PW = pulseIn(sensorOut, LOW);
  // Return the value
  return PW;

}

// Function to read Blue Pulse Widths
int getBluePW() {

  // Set sensor to read Blue only
  digitalWrite(S2, LOW);
  digitalWrite(S3, HIGH);
  // Define integer to represent Pulse Width
  int PW;
  // Read the output Pulse Width
  PW = pulseIn(sensorOut, LOW);
  // Return the value
  return PW;

}
