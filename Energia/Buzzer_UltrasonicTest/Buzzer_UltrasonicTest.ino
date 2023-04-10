#include <msp430.h>

long duration;
int distance;

void setup() {
  // put your setup code here, to run once:
  pinMode(P5_0,INPUT); //echo, Pin 3.0
  pinMode(P5_1,OUTPUT); //trig, Pin 3.2
  pinMode(PUSH2,INPUT_PULLUP); //button (Pin 2.3) triggers
    pinMode(P6_6,OUTPUT); // make the LED pin outpu  
        TB1CCR0 = 384.615384615;                         // PWM Period of 50Hz

    TB1CCTL1 = OUTMOD_7;                      // CCR1 reset/set
    TB1CCR1 = 384.615384615/2;                   // CCR1 PWM duty cycle

    TB1CTL = TBSSEL__SMCLK | MC__UP | TBCLR;  // SMCLK, up mode, clear TBR
        P2DIR |= BIT0;                     // P6.0 output
    P2SEL0 |= BIT0;
    P2SEL1 &= ~BIT0;                    // P6.0 options select
  Serial.begin(9600);
      // previously configured port settings
    PM5CTL0 &= ~LOCKLPM5;

    __bis_SR_register(LPM0_bits + LPM3_bits + GIE);             // Enter LPM0 and LPM3
    __enable_interrupt();                               // Enables interrupts
    __no_operation();                         // For debugger
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
    digitalWrite(P6_6,HIGH);
  }
  else
  {
    digitalWrite(P6_6,LOW);
  }
}
