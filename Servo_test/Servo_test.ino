#include <msp430.h>

void setup() {
  // put your setup code here, to run once:
  WDTCTL = WDTPW | WDTHOLD;                 // Stop WDT
  pinMode(P4_6,OUTPUT); // make the LED pin output
  pinMode(P6_6,OUTPUT);
  P4SEL0 |= BIT6;
  P4SEL1 &= ~BIT6;
  digitalWrite(P4_6,LOW);
  digitalWrite(P6_6,LOW);
  setupTimer(20000); // set timer period to 20000 micro seconds  
    PM5CTL0 &= ~LOCKLPM5;

    __bis_SR_register(LPM0_bits + LPM3_bits + GIE);             // Enter LPM0 and LPM3
    __enable_interrupt();                               // Enables interrupts
    __no_operation();                         // For debugger
}

void loop() {
  // put your main code here, to run repeatedly: 
}

//void OnTimer()
//{
//  static long msCount=0;// Count milliseconds to allow a 1 second pulse
//  static int state=0;  // Remember LED state for toggling purposes
// 
//  msCount++;
//  if (msCount >= 1.5)
//  {
//    msCount = 0;
//    digitalWrite(P6_6,state); // Write to LED
//    state=~state;             // toggle state
//  }
//}

void setupTimer(unsigned Period)
{
  // Configuration word
  // Bits 15-10: Unused
  // Bits 9-8: Clock source select: set to SMCLK (1MHz)
  // Bits 7-6: Input divider: set to 0
  // Bits 5-4: Mode control: Count up to TBCCRO and reset
  // Bit 3: Unused
  // Bit 2: TBCLR : set to initially clear timer system
  // Bit 1: Enable interrupts from TB0
  // Bit 0: Interrupt (pending) flag : set to zero (initially)
    // Configure Timer_B3
    TB0CCR0 = 20000;                         // PWM Period of 50Hz

    TB0CCTL1 = OUTMOD_7;                      // CCR1 reset/set
    TB0CCR1 = 20000*0.075;                   // CCR1 PWM duty cycle

    TB0CTL = TBSSEL__SMCLK | MC__UP | TBCLR;  // SMCLK, up mode, clear TBR
}

//// The address function that follows this vector statement is placed in the specified location Interrupt Vector table 
//#pragma vector=TIMER0_B0_VECTOR
//__interrupt  void timerB0ISR(void)
//{
//// Timer B0 Interrupt service routine
//  OnTimer();
//  TB0CTL &= ~BIT0;     // Acknowledge the interrupt      
//}
