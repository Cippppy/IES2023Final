unsigned long TP2_6 = 0.384615385; // Time period for 2.6kHz
unsigned long halfTP2_6 = TP2_6; // 50% Duty Cycle for 2.6kHz

unsigned long TP2_8 = 0.357142857; // Time period for 2.8kHz
unsigned long halfTP2_8 = TP2_8; // 50% Duty Cycle for 2.8kHz

unsigned long TP2_4 = 0.416666667; // Time period for 2.4kHz
unsigned long halfTP2_4 = TP2_4; // 50% Duty Cycle for 2.4kHz

unsigned char state = 1; // Sets which frequency torun at for testing purposes

void setup() {
  // put your setup code here, to run once:
  pinMode(P1_2,OUTPUT); // Squarewave output 1.2
}

void loop() {
  // put your main code here, to run repeatedly:
  switch(state){
    case 0 : // square wave at 2.6kHz
      digitalWrite(P1_2, HIGH);
      delay(halfTP2_6);
      digitalWrite(P1_2,LOW);
      delay(halfTP2_6);
      break;
    case 1 : // square wave at 2.8kHz
      digitalWrite(P1_2, HIGH);
      delay(halfTP2_8);
      digitalWrite(P1_2,LOW);
      delay(halfTP2_8);
      break;
    case 2 : // square wave at 2.4kHz
      digitalWrite(P1_2, HIGH);
      delay(halfTP2_6);
      digitalWrite(P1_2,LOW);
      delay(halfTP2_6);
      break;
  }
}
