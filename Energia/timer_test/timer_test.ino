#include <msp430.h>
void setup()
{
  // put your setup code here, to run once:
  pinMode(P6_6,OUTPUT); // make the LED pin outpu  
  pinMode(P1_0,OUTPUT);
  setupTimer(1000); // set timer period to 1000 micro seconds  
}
 
void loop()
{
  digitalWrite(P1_0,HIGH);
  delay(100);
  digitalWrite(P1_0,LOW);
  delay(100);
   
}
void OnTimer()
{
  static int msCount=0;// Count milliseconds to allow a 1 second pulse
  static int state=0;  // Remember LED state for toggling purposes
 
  msCount++;
  if (msCount >= 1000)
  {
    msCount = 0;
    digitalWrite(P6_6,state); // Write to LED
    state=~state;             // toggle state
  }
 
}
void setupTimer(unsigned Period)
{
  // Configuration word
  // Bits 15-10: Unused
  // Bits 9-8: Clock source select: set to SMCLK (16MHz)
  // Bits 7-6: Input divider: set to 8
  // Bits 5-4: Mode control: Count up to TACCRO and reset
  // Bit 3: Unused
  // Bits 2: TACLR : set to initially clear timer system
  // Bit 1: Enable interrupts from TA0
  // Bit 0: Interrupt (pending) flag : set to zero (initially)
  TB0CTL=0b0000001011010010; 
  TBCCR0=Period*2; // Set TACCR0 = Period*2 (2MHz clock)
  TBCCTL0=BIT4; // Enable interrupts when TAR = TACCR0  
}
// The address function that follows this vector statement is placed in the specified location Interrupt Vector table 
#pragma vector=TIMER0_B0_VECTOR
__interrupt  void timerB0ISR(void)
{
// Timer B0 Interrupt service routine
  OnTimer();
  TB0CTL &= ~BIT0;     // Acknowledge the interrupt      
}
