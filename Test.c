#include <msp430.h>
#include "GPIO_Driver.h"
#include "FR2355_UART_Driver.h"

void Ultrasonic_Init(void);
void Ultrasonic_ReadDistance(void);

volatile unsigned long distance = 20;    // Holds distance calculated by ultrasonic sensor

void main(void) {

    WDTCTL = WDTPW | WDTHOLD;                 // Stop WDT

    Ultrasonic_Init();

    // Disable the GPIO power-on default high-impedance mode to activate
    // previously configured port settings
    PM5CTL0 &= ~LOCKLPM5;

    __bis_SR_register(LPM0_bits + LPM3_bits + GIE); // Enter LPM0 and LPM3
    __enable_interrupt();   // Enables interrupts
    __no_operation();       // For debugger

    while(1)
       {
           Ultrasonic_ReadDistance();
           if(distance < 15) { // If distance is less than 15 (maybe cm?)
               gpioWrite(1,0,1);   // Turn on Red LED
               gpioWrite(6,6,1);   // Turn on Blue LED
           }
           else {  // If distance is further than 15
               gpioWrite(1,0,0);   // Turn off Red LED
               gpioWrite(6,6,0);   // Turn off Blue LED
           }
       }
}

// Configure Timer_B2 for measuring echo pulse duration
void Ultrasonic_Init(void)
{
    gpioInit(5,1,1); // Set Pin 5.1 to OUTPUT (TRIG)
    gpioWrite(5,1,0); // Set Pin 5.1 to LOW

    gpioInit(5,0,0); // Set Pin 5.0 to INPUT (ECHO)
    P5SEL0 |= BIT0;
    P5SEL1 &= ~BIT0;

    // Set up Timer_B2
    TB2CTL = TBSSEL__SMCLK | MC__CONTINUOUS | TBCLR; // SMCLK, continuous mode, clear TBR
    TB2CCTL0 = CM_3 | CCIS_0 | CAP | CCIE; // Capture mode, CCI0A, both edges, interrupt enabled
}

// Send trigger pulse and measure duration of echo pulse
void Ultrasonic_ReadDistance(void)
{
    // Send trigger pulse
    gpioWrite(5,1,0);   // Set Trig pin LOW
    __delay_cycles(2);  // Delay 5 microseconds
    gpioWrite(5,1,1);   // Set Trig pin HIGH
    __delay_cycles(10); // Delay 10 microseconds
    gpioWrite(5,1,0);   // Set Trig Pin LOW

    // Wait for echo pulse to start
    while ((TB2CCTL0 & CCIFG) == 0); // Wait for first edge

    // Measure duration of echo pulse
    unsigned int risingEdge = TB2CCR0; // Record rising edge time
    TB2CCTL0 &= ~CCIFG; // Clear interrupt flag
    while ((TB2CCTL0 & CCIFG) == 0); // Wait for second edge
    unsigned int fallingEdge = TB2CCR0; // Record falling edge time

    // Calculate distance in cm
    unsigned int pulseWidth = fallingEdge - risingEdge;
    distance = pulseWidth / 58; // Conversion to cm according to datasheet
}

// Timer B2 interrupt service routine (For measuring echo pulse duration)
#pragma vector = TIMER2_B0_VECTOR
__interrupt void Timer2_B0_ISR(void)
{
    TB2CCTL0 &= ~CCIFG; // Clear interrupt flag
}
