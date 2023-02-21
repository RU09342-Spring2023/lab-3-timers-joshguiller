/*
 * Part3.c
 *
 *  Created on: Feb 12, 2023
 *      Author: Joshua Guillermo
 */

#include <msp430.h>

void gpioInit();
void timerInit();

int counter = 3000;
int count;

void main(){

    WDTCTL = WDTPW | WDTHOLD;               // Stop watchdog timer


    gpioInit();
    timerInit();

    // Disable the GPIO power-on default high-impedance mode
    // to activate previously configured port settings
    PM5CTL0 &= ~LOCKLPM5;

    __bis_SR_register(LPM3_bits | GIE);

}

void gpioInit()
{
    // Configure RED LED on P1.0 as Output
          P1OUT &= ~BIT0;                         // Clear P1.0 output latch for a defined power-on state
          P1DIR |= BIT0;                          // Set P1.0 to output direction

          // Configure Button on P2.3 as input with pullup resistor
          P2OUT |= BIT3;                          // Configure P2.3 as pulled-up
          P2REN |= BIT3;                          // P2.3 pull-up register enable
          P2IES &= ~BIT3;                         // P2.3 Low --> High edge
          P2IE |= BIT3;                           // P2.3 interrupt enabled

          P4OUT |= BIT1;
          P4REN |= BIT1;
          P4IES |= BIT1;
          P4IE |= BIT1;

}

void timerInit()
{
    TB1CCTL0 = CCIE;                          // TBCCR0 interrupt enabled
    TB1CCR0 = 50000;
    TB1CTL = TBSSEL_1 | MC_2;                 // ACLK, continuous mode
}

/*
 * INTERRUPT ROUTINES
 */


// Port 2 interrupt service routine
#pragma vector=PORT2_VECTOR
__interrupt void Port_2(void)
{


    P2IFG &= ~BIT3;     // Set interrupt flag to 0

   // counter = 3000;

    if(counter == 3000)     // Speed 1 (fastest)
        {
            counter = 20000;
        }
        else if(counter == 20000)     // Speed 2
        {
            counter = 50000;
        }
        else                    // Speed 3 (slowest)
        {
            counter = 3000;
        }
    }

//Port 4 interrupt service routine
#pragma vector=PORT4_VECTOR
__interrupt void Port_4(void)
{
    P4IES &= ~BIT1;
    P4IE |= BIT1;

    P4IFG &= ~BIT1;

    counter = 50000;
}

// Timer B1 interrupt service routine
#pragma vector = TIMER1_B0_VECTOR
__interrupt void Timer1_B0_ISR(void)
{
    P1OUT ^= BIT0;          // Toggle the red LED
    TB1CCR0 += counter;     // Add an offset equal to the value of counter
}







