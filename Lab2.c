/*
 * File:   main.c
 * Author: <your names here>
 *
 * Lab 2 - Electronic Dice
 * ECE 3301L - Introduction to Microcontrollers Laboratory
 *
 * Description:
 *   Press the button (RB4, active-low) to "roll" two dice.
 *   Each die is shown on 7 LEDs (PORTC = die 1, PORTD = die 2)
 *   using the dice patterns from the lab handout.
 *   Timer0 is used as a random seed source (human press timing).
 */

#include <xc.h>
#include <stdlib.h>
#include <stdint.h>

#include "Configuration.h"

#define _XTAL_FREQ 64000000UL

/*
 * Bit positions:
 *   b0 = upper-left
 *   b1 = upper-right
 *   b2 = middle-left
 *   b3 = center
 *   b4 = middle-right
 *   b5 = lower-left
 *   b6 = lower-right
 */
static const uint8_t dicePatterns[6] = {
    0x08, // : pattern for 1
    0x22, // : pattern for 2
    0x2A, // : pattern for 3
    0x55, // : pattern for 4
    0x5D, // : pattern for 5
    0x77  // : pattern for 6
};

static void init_clock_to_64MHz(void) {
    // TODO: Select the 16 MHz HFINTOSC using OSCCONbits.IRCF
    // TODO: Select the internal oscillator block using OSCCONbits.SCS
    // TODO: Enable the 4x PLL (OSCTUNEbits.PLLEN) -> 64 MHz

     // Select the 16 MHz HFINTOSC using OSCCONbits.IRCF
    OSCCONbits.IRCF = 7;

    // Select the internal oscillator block using OSCCONbits.SCS
    OSCCONbits.SCS = 0;
   
    // TODO: Enable the 4x PLL (OSCTUNEbits.PLLEN) -> 64 MHz
    while (!OSCTUNEbits.PLLEN = 1)
}

static void init_timer0_free_running(void) {
    // TODO: Configure Timer0 as a free-running 16-bit timer:
    //   - internal clock (Fosc/4)
    //   - no prescaler
    //   - clear TMR0H/TMR0L, then turn the timer ON
    // Hint: use the T0CONbits fields. This timer is your entropy source.

       /* Stop Timer0 while configuring it */
    T0CONbits.TMR0ON = 0;

    /* 16-bit mode */
    T0CONbits.T08BIT = 0;

    /* Internal instruction clock: Fosc/4 */
    T0CONbits.T0CS = 0;

    /* T0SE has no effect with internal clock */
    T0CONbits.T0SE = 0;

    /* Prescaler not assigned: 1:1 */
    T0CONbits.PSA = 1;

    /* Clear the 16-bit timer */
    TMR0H = 0;
    TMR0L = 0;

    /* Start free-running Timer0 */
    T0CONbits.TMR0ON = 1;
}

static void init_io(void) {
    // TODO: Make PORTC and PORTD outputs (dice LEDs) and clear their latches.
    // use LAT to clear the output latches
    LATC = 0x00;
    LATD = 0x00;
    // use TRIS to set ports C and D to outputs (TRISx = 0)
    TRISC = 0x00;
    TRISD = 0x00;
    
    // TODO: Make RB4 an input (button, active-low).
    TRISBbits.TRISB4 = 1;

    // TODO: PBADEN=ON means PORTB<5:0> wake up as ANALOG pins.
    //       Force RB4 digital (ANSELB) or the button will always read 0.
    ANSELBbits.ANSB4 = 0;
}

static uint8_t read_button_pressed(void) {
    // TODO: return 1 while the button is pressed (remember: active-low)
    while {
        // active low checks for 0 
        if (PORTBbits.RB4 == 0) {
            // button pressed, send high
            LATDbits.LATD0 = 1;
        }
        else{
            //button released, send low
            LATDbits.LATD0 = 0;
        }
    }
    return 0;
}

static void debounce_press(void) {
    // TODO: short delay (~30 ms) to ride out contact bounce
}

static void wait_for_release(void) {
    // TODO: block until the button is released, then debounce the release
}

void main(void) {
    init_clock_to_64MHz();
    init_io();
    init_timer0_free_running();

    while (1) {
        // TODO: When a (debounced) press is detected:
        //   1. Read TMR0H:TMR0L and use it to seed rand() with srand()
        //   2. Roll two dice with rand() % 6
        //   3. Show the two patterns on LATC and LATD
        //   4. Keep them displayed for 3 seconds, then turn LEDs off
        //   5. Wait for the button to be released (one roll per press)
    }
}
