#include <msp430.h>
#include "GPIO_Driver.h"


/**
 * main.c
 */
int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	gpioInit(1,0,1);
	gpioWrite(1,0,0);
	while(1)
	{
	    gpioWrite(1,0,1);
	    __delay_cycles(100000);
	    gpioWrite(1,0,0);
	    __delay_cycles(100000);
	}
	return 0;
}
