long duration;
int distance;

void setup() {
  // put your setup code here, to run once:
  pinMode(P3_0,INPUT); //echo
  pinMode(P3_2,OUTPUT); //trig
  pinMode(PUSH2,INPUT_PULLUP); //button triggers
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

  Serial.print(distance);   // Prints the distance on the Serial Monitor
  Serial.print("\n");
  delay(1000);

  
}
