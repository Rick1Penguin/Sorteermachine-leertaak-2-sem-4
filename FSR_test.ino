

void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
pinMode(A0,INPUT);
pinMode(7,OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(7,LOW);
  int Status =analogRead(A0);
  Serial.println(Status);
if (Status>=5){
  Serial.print(Status);
  Serial.println("  detected");
  digitalWrite(7,HIGH);
  delay(2000);
}
}
