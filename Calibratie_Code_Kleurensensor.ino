
#include <Servo.h> //Servo library toegevoegd

Servo knikkerServoG; //detectie servo wiel voor Grote knikkers
Servo sorteerServoG; // de sorteerservo voor de grote knikkers

const uint8_t knikkerServoGPin = 8; //servo pin
const uint8_t sorteerServoGPin = 6; //servo pin

const uint8_t S0 = 53;  //kleurensensor pin
const uint8_t S1 = 51;  //kleurensensor pin
const uint8_t S2 = 41;  //kleurensensor pin
const uint8_t S3 = 43;  //kleurensensor pin
const uint8_t sensorOut = 39;  //kleurensensor pin

int redPW = 0; // Variables for Color Pulse Width Measurements
int greenPW = 0;
int bluePW = 0;
int redValueMin; //variabeles voor knikkerdetectie
int redValueMax;
int greenValueMin;
int greenValueMax;
int blueValueMin;
int blueValueMax;
int redValue; //variabeles voor knikkerdetectie
int greenValue;
int blueValue;

//Kleurensensor standaard:
const int redMin = 28; // Red minimum value
const int redMax = 178; // Red maximum value
const int greenMin = 32; // Green minimum value
const int greenMax = 199; // Green maximum value
const int blueMin = 28; // Blue minimum value
const int blueMax = 160; // Blue maximum value

const uint8_t knikkerServo_Begin = 18;
const uint8_t knikkerServo_Sensor = 80;
const uint8_t knikkerServo_Eind = 144;
const uint8_t sorteerServoGhoekMidden = 40; //de buffer voor midden

void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
  //Alle pinmodes worden hier gedefineerd:
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  pinMode(sensorOut, INPUT);
  pinMode(knikkerServoGPin, OUTPUT);
  pinMode(sorteerServoGPin, OUTPUT);

  //Alle Servos worden verbinden met de juiste pin:
  knikkerServoG.attach(knikkerServoGPin);
  sorteerServoG.attach(sorteerServoGPin);

  // Set Frequency scaling to 20%
  digitalWrite(S0, HIGH);
  digitalWrite(S1, LOW);
  
  Serial.println("Setup Complete.");
}

void loop() {
    // wiel draaien naar beginpositie en naar de sensor:
    Serial.println("meten grote knikker");
    knikkerServoG.write(knikkerServo_Begin);
    delay(1000);
    for (int hoek = knikkerServo_Begin; hoek <= knikkerServo_Sensor; hoek++) {
      knikkerServoG.write(hoek); //per stap van 1 graden wordt de servo aangestuurd
      delay(3.7);
    }
    delay(1000);

redValueMin=1000; //variabeles voor knikkerdetectie
redValueMax=0;
greenValueMin=1000;
greenValueMax=0;
blueValueMin=1000;
blueValueMax=0;

for(int i=0; i<5; i++){
  Serial.println("meting gedaan");
      delay(200);// Delay to stabilize sensor
      redPW = getRedPW();  // Read Red value
      redValue = map(redPW, redMin, redMax, 255, 0);  // Map to value from 0-255
      
      delay(200); // Delay to stabilize sensor
      greenPW = getGreenPW();  // Read Green value
      greenValue = map(greenPW, greenMin, greenMax, 255, 0);  // Map to value from 0-255
      
      delay(200); // Delay to stabilize sensor
      bluePW = getBluePW();  // Read Blue value
      blueValue = map(bluePW, blueMin, blueMax, 255, 0);  // Map to value from 0-255

      if(redValue>redValueMax){
        redValueMax=redValue;
      }
      if(redValue<redValueMin){
        redValueMin=redValue;
      }
      
      if(greenValue>greenValueMax){
        greenValueMax=greenValue;
      }
      if(greenValue<greenValueMin){
        greenValueMin=greenValue;
      }
      
      if(blueValue>blueValueMax){
        blueValueMax=blueValue;
      }
      if(blueValue<blueValueMin){
        blueValueMin=blueValue;
      }
      


}

      sorteerServoG.write(sorteerServoGhoekMidden); 

      for (int hoek = knikkerServo_Sensor; hoek <= knikkerServo_Eind; hoek++) {
        knikkerServoG.write(hoek); //per stap van 1 graden wordt de servo aangestuurd
        delay(3.7);
      }
      delay(1000);
        // Print output to Serial Monitor
    Serial.print("RedMin = ");
    Serial.print(redValueMin);
    Serial.print(" - RedMax = ");
    Serial.println(redValueMax);
    Serial.print("GreenMin = ");
    Serial.print(greenValueMin);
    Serial.print(" - GreenMax = ");
    Serial.println(greenValueMax);
    Serial.print("BlueMin = ");
    Serial.print(blueValueMin); 
    Serial.print(" - BlueMax = ");
    Serial.println(blueValueMax);     
    
    delay(2000);
      for (int hoek = knikkerServo_Eind; hoek >= knikkerServo_Begin; hoek--) {
        knikkerServoG.write(hoek); //per stap van 1 graden wordt de servo aangestuurd
        delay(3.7);
      }
      delay(5000);
}

// Function to read Red Pulse Widths bij de grote knikkers
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

// Function to read Green Pulse Widths bij de grote knikkers
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

// Function to read Blue Pulse Widths bij de grote knikkers
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
