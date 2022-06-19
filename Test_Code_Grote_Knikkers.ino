#include <Servo.h>


Servo knikkerServoG;
Servo sorteerServoG; // de sorteerservo voor de grote knikkers

//servo pinnen:
const uint8_t sorteerServoGPin = 10; //servo pin
const uint8_t knikkerServoGPin = 3; //servo pin

//kleurensensor pinnen:
const uint8_t S0 =9;
const uint8_t S1 =13;
const uint8_t S2 =12;
const uint8_t S3 =11;
const uint8_t sensorOut =10;

// Calibration Values voor sensor
// Get these from Calibration Sketch sensor
const int redMin = 28; // Red minimum value
const int redMax = 178; // Red maximum value
const int greenMin = 32; // Green minimum value
const int greenMax = 199; // Green maximum value
const int blueMin = 28; // Blue minimum value
const int blueMax = 160; // Blue maximum value

// Variables for Color Pulse Width Measurements
int redPW = 0;
int greenPW = 0;
int bluePW = 0;

int redValue;
int greenValue;
int blueValue;

//Servo variables:
const int knikkerServo_Begin = 18;
const int knikkerServo_Sensor = 80;
const int knikkerServo_Eind = 144;
const int sorteerServoGhoekPlastic = 69; //de buffer voor plastic
const int sorteerServoGhoekHout = 0; //de buffer voor hout

//Callibratie Kleuren sensor hout en plastic en geen:
const int Hout_RedLaag = 40;
const int Hout_RedHoog = 70;

const int Hout_GreenLaag = 30;
const int Hout_GreenHoog = 60;

const int Hout_BlueLaag = 30;
const int Hout_BlueHoog = 50;

const int Plastic_RedLaag = 100;
const int Plastic_RedHoog = 130;

const int Plastic_GreenLaag = 110;
const int Plastic_GreenHoog = 140;

const int Plastic_BlueLaag = 100;
const int Plastic_BlueHoog = 130;

void setup() {
  //alle servo's
  knikkerServoG.attach(knikkerServoGPin);
  sorteerServoG.attach(sorteerServoGPin);
  
  //alle Outputs
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  pinMode(sorteerServoGPin, OUTPUT);
  pinMode(knikkerServoGPin, OUTPUT);
  
  // alle Inputs
  pinMode(sensorOut, INPUT);
  
  // Set Frequency scaling to 20%
  digitalWrite(S0, HIGH);
  digitalWrite(S1, LOW);

  //beginpositie:
 knikkerServoG.write(knikkerServo_Begin);
 sorteerServoG.write(sorteerServoGhoekHout);
 
  //begin seriële monitor
  Serial.begin(9600);
  
  Serial.println("Setup Complete.");
}

void loop() {
  // put your main code here, to run repeatedly:
 // begin rotatie
 knikkerServoG.write(knikkerServo_Begin);
 delay(2000);
    for(int hoek = knikkerServo_Begin; hoek <= knikkerServo_Sensor; hoek++){ 
      knikkerServoG.write(hoek); //per stap van 1 graden wordt de servo aangestuurd
      delay(3.7);
    }
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
  if (redValue > Hout_RedLaag && redValue < Hout_RedHoog && greenValue > Hout_GreenLaag && greenValue < Hout_GreenHoog && blueValue > Hout_BlueLaag && blueValue < Hout_BlueHoog) {
    Serial.println(" hout ");
    sorteerServoG.write(sorteerServoGhoekHout); //sturen naar de hout buffer
    for(int hoek = knikkerServo_Sensor; hoek <= knikkerServo_Eind; hoek++){ 
      knikkerServoG.write(hoek); //per stap van 1 graden wordt de servo aangestuurd
      delay(3.7);
    }
    delay(2000);
    for(int hoek = knikkerServo_Eind; hoek >= knikkerServo_Begin; hoek--){ 
      knikkerServoG.write(hoek); //per stap van 1 graden wordt de servo aangestuurd
      delay(3.7);
    }
  }
  
  // plastic knikker
  if (redValue > Plastic_RedLaag && redValue < Plastic_RedHoog && greenValue > Plastic_GreenLaag && greenValue < Plastic_GreenHoog && blueValue > Plastic_BlueLaag && blueValue < Plastic_BlueHoog) {
    Serial.println(" plastic ");
    sorteerServoG.write(sorteerServoGhoekPlastic); //sturen naar de plastic buffer
    for(int hoek = knikkerServo_Sensor; hoek <= knikkerServo_Eind; hoek++){ 
      knikkerServoG.write(hoek); //per stap van 1 graden wordt de servo aangestuurd
      delay(3.7);
    }
    delay(2000);
    for(int hoek = knikkerServo_Eind; hoek >= knikkerServo_Begin; hoek--){ 
      knikkerServoG.write(hoek); //per stap van 1 graden wordt de servo aangestuurd
      delay(3.7);
    }
  }
  else {
    Serial.println(" geen knikker ");
    for(int hoek = knikkerServo_Sensor; hoek >= knikkerServo_Begin; hoek--){ 
      knikkerServoG.write(hoek); //per stap van 1 graden wordt de servo aangestuurd
      delay(3.7);
    }
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
