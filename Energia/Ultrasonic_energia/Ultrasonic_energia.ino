long duration;
int distance;

void setup() {
  // put your setup code here, to run once:
  pinMode(P5_0,INPUT); //echo, Pin 3.0
  pinMode(P5_1,OUTPUT); //trig, Pin 3.2
  pinMode(P1_0,OUTPUT);
  pinMode(P6_6,OUTPUT);
  pinMode(PUSH2,INPUT_PULLUP); //button (Pin 2.3) triggers
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly: 
    // Clears the trigPin
  digitalWrite(P5_1, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(P5_1, HIGH);
  delayMicroseconds(10);
  digitalWrite(P5_1, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(P5_0, HIGH);
  // Calculating the distance
  distance = duration * 0.034 / 2;
  // Prints the distance on the Serial Monitor
  if(distance < 5)
  {
    Serial.print("ALERT");
    Serial.print("\n");
    digitalWrite(P1_0,HIGH);
    digitalWrite(P6_6,HIGH);
  }
  else
  {
    digitalWrite(P1_0,LOW);
    digitalWrite(P6_6,LOW);
  }
}
