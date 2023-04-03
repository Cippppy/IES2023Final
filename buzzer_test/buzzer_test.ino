#include <msp430.h>

void BuzzerInit();
void TimerB1Init();

int main(void)
{
  WDTCTL = WDTPW | WDTHOLD;                 // Stop WDT
  BuzzerInit();
  TimerB1Init();

    // Disable the GPIO power-on default high-impedance mode to activate
    // previously configured port settings
    PM5CTL0 &= ~LOCKLPM5;

    __bis_SR_register(LPM0_bits + LPM3_bits + GIE);             // Enter LPM0 and LPM3
    __enable_interrupt();                               // Enables interrupts
    __no_operation();                         // For debugger
}

// Method to initialize Buzzer pin
void BuzzerInit() {
    P2DIR |= BIT0;                     // P6.0 output
    P2SEL0 |= BIT0;
    P2SEL1 &= ~BIT0;                    // P6.0 options select
}

// Method to initialize TimerB1 which does the duty cycle
void TimerB1Init() {
    // Configure Timer_B1
    TB1CCR0 = 384.615384615;                         // PWM Period of 50Hz

    TB1CCTL1 = OUTMOD_7;                      // CCR1 reset/set
    TB1CCR1 = 384.615384615/2;                   // CCR1 PWM duty cycle

    TB1CTL = TBSSEL__SMCLK | MC__UP | TBCLR;  // SMCLK, up mode, clear TBR
}



/*
 * With Delays
 */

//unsigned long TP2_6 = 0.384615385; // Time period for 2.6kHz
//unsigned long halfTP2_6 = TP2_6; // 50% Duty Cycle for 2.6kHz
//
//unsigned long TP2_8 = 0.357142857; // Time period for 2.8kHz
//unsigned long halfTP2_8 = TP2_8; // 50% Duty Cycle for 2.8kHz
//
//unsigned long TP2_4 = 0.416666667; // Time period for 2.4kHz
//unsigned long halfTP2_4 = TP2_4; // 50% Duty Cycle for 2.4kHz
//
//unsigned char state = 1; // Sets which frequency torun at for testing purposes
//
//void setup() {
//  // put your setup code here, to run once:
//  pinMode(P1_2,OUTPUT); // Squarewave output 1.2
//}
//
//void loop() {
//  // put your main code here, to run repeatedly:
//  switch(state){
//    case 0 : // square wave at 2.6kHz
//      digitalWrite(P1_2, HIGH);
//      delay(halfTP2_6);
//      digitalWrite(P1_2,LOW);
//      delay(halfTP2_6);
//      break;
//    case 1 : // square wave at 2.8kHz
//      digitalWrite(P1_2, HIGH);
//      delay(halfTP2_8);
//      digitalWrite(P1_2,LOW);
//      delay(halfTP2_8);
//      break;
//    case 2 : // square wave at 2.4kHz
//      digitalWrite(P1_2, HIGH);
//      delay(halfTP2_6);
//      digitalWrite(P1_2,LOW);
//      delay(halfTP2_6);
//      break;
//  }
//}
