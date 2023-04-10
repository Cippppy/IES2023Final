#include <msp430.h>
#include "GPIO_Driver.h"
#include "FR2355_UART_Driver.h"

volatile unsigned int distance;

void Init_HCSR04();
void Init_Timer();

/**
 * Ultrasonic.c
 */
int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5;

    Init_HCSR04();
    Init_Timer();

    __bis_SR_register(GIE); // Enable global interrupts

    while (1)
    {
        P5OUT &= ~BIT1;
        __delay_cycles(2);
        P5OUT |= BIT1;
        __delay_cycles(100);
        P5OUT &= ~BIT1;

        __delay_cycles(200000); // Wait for the echo pulse to settle down

        unsigned long t = (TB0CCR1 > TB0CCR0) ? TB0CCR1 - TB0CCR0 : 0; // Calculate the pulse width
        distance = (unsigned int) (t / 58.0); // Calculate the distance in cm
    }
}

void Init_HCSR04()
{
    P5DIR |= BIT1;
    P5DIR &= ~BIT0;
    P5SEL0 |= BIT0;
    P5SEL1 &= ~BIT0;
    P5REN |= BIT0;
    P5OUT |= BIT0;
}

void Init_Timer()
{
    TB2CTL = TBSSEL__SMCLK | MC__UP | TBCLR; // Selecting SMCLK as Timer B source and UP mode
    TB2CCTL1 = CM_3 | CCIS_0 | CAP | CCIE; // Capture mode, Rising and Falling edge detection and interrupt enabled
}

#pragma vector = TIMER0_B1_VECTOR
__interrupt void Timer_B1(void)
{
    if (TB2CCTL1 & CCI)
    {
        TB2CCR0 = 0xFFFF; // Resetting the counter
        TB2CCTL1 &= ~CCI; // Clearing the interrupt flag
    }
    else
    {
        TB2CCR1 = TB2R; // Storing the counter value when the echo pin goes high
        TB2CCTL1 |= CCI; // Setting the interrupt flag for the falling edge
    }
}
