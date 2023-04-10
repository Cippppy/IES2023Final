void setup() {
  // put your setup code here, to run once:
  pinMode(P3_7,OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly: 
  digitalWrite(P3_7,HIGH);
  delay(10);
  digitalWrite(P3_7,LOW);
  delay(1000);
}
