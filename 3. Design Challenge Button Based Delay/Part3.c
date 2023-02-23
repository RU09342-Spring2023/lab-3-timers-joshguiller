/*
 * Part3.c
 *
 *  Created on: Feb 12, 2023
 *      Author: Joshua Guillermo
 */

#include <msp430.h>

void gpioInit();
void timerInit();

int ifCount = 0;
int press = 3000;

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

          P4OUT |= BIT1;                          // Configure P4.1 as pulled-up
          P4REN |= BIT1;                          // P4.1 pull-up register enable
          P4IES &= ~BIT1;                         // P4.1 Low --> High edge
          P4IE |= BIT1;                           // P4.1 interrupt enabled
}

void timerInit()
{
    //Initializing timer for blinking of LED
    TB0CCTL0 = CCIE;                                 // TBCCR0 interrupt enabled
    TB0CCR0 = press;
    TB0CTL = TBSSEL_1 | MC_2 | ID_3;                 // ACLK, continuous mode, divided by 8

    //Initializing timer for the length of button press
    TB1CCTL0 = CCIE;                          // TBCCR1 interrupt enabled
    TB1CCR1 = 1;
    TB1CTL = TBSSEL_1 | MC_2 | ID_3;                // ACLK, continuous mode, divided by 8

}

/*
 * INTERRUPT ROUTINES
 */

// Port 2 interrupt service routine
#pragma vector=PORT2_VECTOR
__interrupt void Port_2(void)
{
    P2IFG &= ~BIT3;     // Set interrupt flag to 0

    if(P2IES & BIT3)    // If falling edge (button press)
    {
        ifCount = 1;        // Button pressed = 1
        press = 0;          // Resets the button pressed timer
        P2IES &= ~BIT3;     // Look for rising edge
    }
    else if (!(P2IES & BIT3))   // If rising edge (button release)
    {
        ifCount = 0;            // Button pressed = 0
        P2IES |= BIT3;          // Look for falling edge
        press = press;
    }
}

//Port 4 interrupt service routine (Resets the timer)
#pragma vector=PORT4_VECTOR
__interrupt void Port_4(void)
{
    P4IFG &= ~BIT1;     // Set interrupt flag to 0

    press = 3000;       // Reset frequency to original
    ifCount = 0;        // Button pressed = 0
}

// Timer B0 interrupt service routine (Timer for LED blinks)
#pragma vector = TIMER0_B0_VECTOR
__interrupt void Timer0_B0_ISR(void)
{
    P1OUT ^= BIT0;         // Toggle the red LED
    TB0CCR0 += press;     // Add an offset equal to the value of press
}

// Timer B1 interrupt service routine   (Timer for length of button press)
#pragma vector = TIMER1_B1_VECTOR
__interrupt void Timer1_B1_ISR(void)
{
    if(ifCount == 1)      // If the button is pressed
    {
        press += 1;          // Add to the press timer
    }
    else if (ifCount == 0)      // If button is not pressed
    {
        press = press;
    }
    //TB1CCR1 += 1;
}
