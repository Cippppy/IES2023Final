#include <msp430.h>
#include "GPIO_Driver.h"
#include "FR2355_UART_Driver.h"

/**
 * Final.c
 */

// Variable Instantiation
char Vibration_Data_In;
char Temperature_Data_In;
unsigned char state = 0;    // Holds state value if we do state machine

// Text that prints to terminal if distance < 15
char alertText[] = {'D','A','N','G','E','R', ' ','N','E','A','R'};

// Text that prints to terminal when uart connects
char connectedText[] = {'S','u','c','c','e','s','s','f','u','l','l','y',' ','C','o','n','n','e','c','t','e','d'};

volatile unsigned long distance = 20;    // Holds distance calculated by ultrasonic sensor
volatile unsigned int pulseCount = 0;   // Holds count used to calculate distance

// Method Instantiation
void Init_HCSR04();
void Init_Buzzer();
void Init_Servo();
void Init_LED();
void Init_WIFI();
void Init_TEMP();
void Init_CO2();
void Init_UART_Pins();
void Run_Ultrasonic();
void Ultrasonic_Wait(unsigned int timeToWait);


void main(void)
{
    WDTCTL = WDTPW | WDTHOLD;                 // Stop WDT
    Init_HCSR04();
    Init_Buzzer();
    Init_Servo();
    Init_LED();
    Init_WIFI();
    Init_TEMP();
    Init_CO2();
    Init_UART_Pins();
    uart_Init_9600();
    uart_Print(connectedText);

    // Disable the GPIO power-on default high-impedance mode to activate
    // previously configured port settings
    PM5CTL0 &= ~LOCKLPM5;

    __bis_SR_register(LPM0_bits + LPM3_bits + GIE); // Enter LPM0 and LPM3
    __enable_interrupt();   // Enables interrupts
    __no_operation();       // For debugger

    Ultrasonic_Wait(65000); // Wait some time (Maybe 65000 ms)
    while(1)
    {
        Run_Ultrasonic();
        if(distance < 15) { // If distance is less than 15 (maybe cm?)
            gpioWrite(3,2,1);   // Turn on Red LED
            gpioWrite(3,3,1);   // Turn on Blue LED
            gpioWrite(3,7,1);   // Turn on Buzzer
            uart_Print(alertText);  // Print "DANGER NEAR" to terminal
        }
        else {  // If distance is further than 15
            gpioWrite(3,2,0);   // Turn off Red LED
            gpioWrite(3,3,0);   // Turn off Blue LED
            gpioWrite(3,7,0);   // Turn off buzzer
        }
        Ultrasonic_Wait(65000); // Wait some time (Maybe 65000 ms)
    }
}


/**
 * Peripheral Setups
 */

// Setting up pins for Computer terminal connection
void Init_UART_Pins()
{
    P4DIR = 0xFF; P2DIR = 0xFF;
    P4REN = 0xFF; P2REN = 0xFF;
    P4OUT = 0x00; P2OUT = 0x00;
    // Configure UART pins
    P4SEL0 |= BIT2 | BIT3;                    // set 2-UART pin as second function
}


// Setup Ultrasonic sensor to Pins 5.1 and 5.0 and Timer B2
void Init_HCSR04()
{
    gpioInit(5,1,1); // Set Pin 5.1 to OUTPUT (TRIG)
    gpioWrite(5,1,0); // Set Pin 5.1 to LOW

    gpioInit(5,0,0); // Set Pin 5.0 to INPUT (ECHO)
    P5SEL0 |= BIT0;
    P5SEL1 &= ~BIT0;

    // Configure Timer_B2
    TB2CCR0 = 50;   // Set to 50 because anything lower is too fast
    TB2CCTL0 = CCIE;    // TB2CCR0 interrupt enabled
    TB2CTL = TBSSEL__SMCLK | MC_2 | TBCLR;    // SMCLK, continuous mode, clear TBR
}

// Setup Buzzer to Pin 3.7
void Init_Buzzer()
{
    gpioInit(3,7,1); // Set Pin 3.7 to OUTPUT (Transistor Base)
    gpioWrite(3,7,0); // Set Pin 3.7 to LOW
}

// Setup Servo to Pin 6.0 and Timer B3
void Init_Servo()
{
    gpioInit(6,0,1); // Set Pin 6.0 to OUTPUT (Servo PWM)
    P6SEL0 |= BIT0;
    P6SEL1 &= ~BIT0; // P6.0 options select

    // Configure Timer_B3 for PWM
    TB3CCR0 = 20000;                         // PWM Period of 50Hz
    TB3CCTL1 = OUTMOD_7;                      // CCR1 reset/set
    TB3CCR1 = 20000*0.075;                   // CCR1 PWM duty cycle
    TB3CTL = TBSSEL__SMCLK | MC__UP | TBCLR;  // SMCLK, up mode, clear TBR
}

// Setup LEDs to Pin 3.2 and 3.3
void Init_LED()
{
    gpioInit(3,2,1); // Set Pin 3.2 to OUTPUT (RED LED)
    gpioWrite(3,2,0); // Set Pin 3.2 to LOW

    gpioInit(3,3,1); // Set Pin 3.3 to OUTPUT (BLUE LED)
    gpioWrite(3,3,0); // Set Pin 3.3 to LOW
}

// Setup Wifi Module
void Init_WIFI()
{

}

// Setup Temperature Sensor to I2C Pins 1.6 and 1.7
void Init_TEMP()
{
    UCB1CTLW0 |= UCSWRST; // Set USCI_B1 into software reset

    UCB1CTLW0 |= UCSSEL_3; // Choose BRCLK=SMCLK=1MHz
    UCB1BRW = 10; // Divide BRCLK by 10 for SCL = 100kHz
    UCB1CTLW0 |= UCMODE_3 | UCMST; // Put into I2C mode, Put into Master mode
    UCB1I2CSA = 0x0068; // Slave address = 0x68
    UCB1CTLW1 |= UCASTP_2; //Auto STOP when UCB0TBCNT reached
    UCB1TBCNT = 0x01; // Send 1 byte of data

    // SCL Pin Setup
    P4SEL0 |= BIT7;
    P4SEL1 &= ~BIT7;

    // SDA Pin Setup
    P4SEL0 |= BIT6;
    P4SEL1 &= ~BIT6;

    UCB1CTLW0 &= ~UCSWRST; // Take USCI_B0 out of software reset

    UCB1IE |= UCTXIE0; // Enable I2C Tx0 IRQ (writing interrupt)
    UCB1IE |= UCRXIE0; // Enable I2C Rx0 IRQ (receiving interrupt)
}

// Setup Vibration sensor to I2C Pins 1.2 and 1.3
void Init_Vibration()
{
    UCB0CTLW0 |= UCSWRST; // Set USCI_B0 into software reset

    UCB0CTLW0 |= UCSSEL_3; // Choose BRCLK=SMCLK=1MHz
    UCB0BRW = 10; // Divide BRCLK by 10 for SCL = 100kHz
    UCB0CTLW0 |= UCMODE_3 | UCMST; // Put into I2C mode, Put into Master mode
    UCB0I2CSA = 0x0068; // Slave address = 0x68
    UCB0CTLW1 |= UCASTP_2; //Auto STOP when UCB0TBCNT reached
    UCB0TBCNT = 0x01; // Send 1 byte of data

    // SCL Pin Setup
    P1SEL0 |= BIT3;
    P1SEL1 &= ~BIT3;

    // SDA Pin Setup
    P1SEL0 |= BIT2;
    P1SEL1 &= ~BIT2;

    UCB0CTLW0 &= ~UCSWRST; // Take USCI_B0 out of software reset

    UCB0IE |= UCTXIE0; // Enable I2C Tx0 IRQ (writing interrupt)
    UCB0IE |= UCRXIE0; // Enable I2C Rx0 IRQ (receiving interrupt)
}

// Setup CO2 Sensor to Pin 1.1 and ADC
void Init_CO2()
{
    gpioInit(1,1,0); // Set Pin 1.1 to INPUT (C02 Sensor)
}


/**
 * Peripheral Functions
 */

void Run_Ultrasonic()
{
    gpioWrite(5,1,0);   // Set Trig pin LOW
    __delay_cycles(2);  // Delay 5 microseconds
    gpioWrite(5,1,1);   // Set Trig pin HIGH
    __delay_cycles(10); // Delay 10 microseconds
    gpioWrite(5,1,0);   // Set Trig Pin LOW
    pulseCount = 0;
    unsigned int countLow = 0;   // Set count of LOW part of echo pulse to 0
    unsigned int countHigh = 0;  // Set count of HIGH part of echo pulse to 0
    while(gpioRead(5,0) == 0);
    countLow = pulseCount * 5;   // Set count of LOW part of echo pulse to pulseCount times 5 because of TB2CCR0 offset
    while(gpioRead(5,0) == 1);
    countHigh = pulseCount * 5;  // Set count of HIGH part of echo pulse to pulseCount times 5 because of TB2CCR0 offset
    unsigned int pulseWidth = countHigh - countLow; // Pulse width = total count - count of LOW part
    distance = (unsigned long) pulseWidth / 58.0;   // Conversion to cm according to datasheet
}

// Will be used to set servo angle
void setServo()
{
    TB3CCR1 = 0.0;
}

// Used to make the ultrasonic wait without using delay
void Ultrasonic_Wait(unsigned int timeToWait)
{
    unsigned int timeWaited = pulseCount;
    while((pulseCount - timeWaited) < timeToWait);
}


/**
 * Interrupt Routines
 */

// Interrupt routine for Vibration sensor
#pragma vector=EUSCI_B0_VECTOR
__interrupt void EUSCI_B0_I2C_ISR(void)
{
    switch(UCB0IV) // Which IRQ flag triggered
    {
    case 0x16 : // RXIFG0
        Vibration_Data_In = UCB0RXBUF; // Runs when Rx buffer has received data from the slave.
        break;
    case 0x18 : // TXIFG0
        UCB0TXBUF = 0xBB; // Runs when Tx buffer is ready to receive data from master.
        break;
    default :
        break;
    }
}

// Interrupt routine for Temperature sensor
#pragma vector=EUSCI_B1_VECTOR
__interrupt void EUSCI_B1_I2C_ISR(void)
{
    switch(UCB1IV) // Which IRQ flag triggered
    {
    case 0x16 : // RXIFG0
        Temperature_Data_In = UCB1RXBUF; // Runs when Rx buffer has received data from the slave.
        break;
    case 0x18 : // TXIFG0
        UCB1TXBUF = 0xBB; // Runs when Tx buffer is ready to receive data from master.
        break;
    default :
        break;
    }
}

// Timer B2 interrupt service routine (For counting how long echo is high)
#pragma vector = TIMER2_B0_VECTOR
__interrupt void Timer2_B0_ISR(void)
{
    if(TB2CCR0 >= 65100) {
        TB2CCR0 = 50;
    }
    pulseCount++;
    TB2CCR0 += 50;               // Add Offset to TB0CCR0 (anything smaller than 50 is too fast)
}
