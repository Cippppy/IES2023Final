long duration;
int distance;

void setup() {
  // put your setup code here, to run once:
  pinMode(P1_2,OUTPUT); // LEDPIN
  pinMode(P3_0,INPUT); //echo
  pinMode(P3_2,OUTPUT); //trig
  pinMode(PUSH2,INPUT_PULLUP); //button triggers
  pinMode(P6_6, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly: 
  digitalWrite(P3_2, LOW);     // Clears the trigPin  
  delayMicroseconds(2);

  digitalWrite(P3_2, HIGH);
  delayMicroseconds(10);   // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(P3_2, LOW);

  duration = pulseIn(P3_0, HIGH);   // Reads the echoPin, returns the sound wave travel time in microseconds

  distance = duration * 0.034 / 2;   // Calculating the distance

  if(distance<14)
  {
    Serial.print("ALERT");
    Serial.print("\n");
    digitalWrite(P1_2,HIGH);
    digitalWrite(P6_6, LOW);
    delay(500);
    digitalWrite(P1_2, LOW);
    delay(100);
  }
  else
  {
  Serial.print(distance);   // Prints the distance on the Serial Monitor
  Serial.print("\n");
  digitalWrite(P6_6, HIGH);
  digitalWrite(P1_2, LOW);
  delay(1000);
  digitalWrite(P6_6, LOW);
  delay(500);
  }

}
