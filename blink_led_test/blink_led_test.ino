   

void setup() {
  // put your setup code here, to run once:
  pinMode(P1_0,OUTPUT);
  pinMode(PUSH1,INPUT_PULLUP);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(digitalRead(PUSH1) == LOW) 
  {
  digitalWrite(P1_0,HIGH);
  delay(1000);
  digitalWrite(P1_0,LOW);
  delay(1000);
  }
}
