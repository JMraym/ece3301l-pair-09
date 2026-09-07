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
}

static void init_timer0_free_running(void) {
    // TODO: Configure Timer0 as a free-running 16-bit timer:
    //   - internal clock (Fosc/4)
    //   - no prescaler
    //   - clear TMR0H/TMR0L, then turn the timer ON
    // Hint: use the T0CONbits fields. This timer is your entropy source.
}

static void init_io(void) {
    // TODO: Make PORTC and PORTD outputs (dice LEDs) and clear their latches.

    // TODO: Make RB4 an input (button, active-low).

    // TODO: PBADEN=ON means PORTB<5:0> wake up as ANALOG pins.
    //       Force RB4 digital (ANSELB) or the button will always read 0.
}

static uint8_t read_button_pressed(void) {
    // TODO: return 1 while the button is pressed (remember: active-low)
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
