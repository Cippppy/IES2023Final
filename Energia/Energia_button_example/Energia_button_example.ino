void setup() {
  // put your setup code here, to run once:
  pinMode(P1_0,OUTPUT); // Red LED (Pin 1.0)
  pinMode(PUSH1,INPUT_PULLUP); // Button (Pin 4.1)
}

void loop() {
  // put your main code here, to run repeatedly:
  if(digitalRead(PUSH1) == LOW) // If Button is pressed
  {
    digitalWrite(P1_0,HIGH); // Turn on Red LED
    delay(1000); // Wait
    digitalWrite(P1_0,LOW); // Turn off Red LED
    delay(1000); // Wait
  }
}
