/*
 * File:   lab4.c
 * Author: Julian Barbosa, John Raymundo
 *
 * Lab 4 - Traffic Light Controller
 *         (FSM + Safety Mode + Pedestrian Crossing)
 * ECE 3301L - Introduction to Microcontrollers Laboratory
 *
 * Description:
 *   Formal FSM-based traffic light controller with:
 *   - All-Red flashing safety mode (on-board button S2 = RC5)
 *   - Pedestrian crossing with 9-to-0 countdown on a 7-segment display
 *     (external SPST button on RE0)
 *   The pedestrian request is LATCHED and serviced after the EW Yellow
 *   phase. During the countdown, NS has Green and EW has Red.
 *
 * Pin Assignments:
 *   PORTD - South (RD5=G, RD4=Y, RD3=R) and North (RD2=G, RD1=Y, RD0=R)
 *   PORTB - East  (RB5=G, RB4=Y, RB3=R) and West  (RB2=G, RB1=Y, RB0=R)
 *           RB6/RB7 preserved (PGC/PGD for LVP programming)
 *   PORTC - 7-Segment Display (common anode, LOW = ON)
 *           RC0=a, RC1=b, RC2=c, RC3=d, RC4=e, RC6=f, RC7=g
 *           RC5 = On-board button S2 (safety, active-low)
 *   PORTE - RE0 = External pedestrian button (active-low, 10k pull-up)
 *   PORTA - RA4 = On-board LED (pedestrian request indicator)
 *
 * FSM States:
 *   EW_GREEN      - EW Green,  NS Red         (6 s)   -> EW_YELLOW
 *   EW_YELLOW     - EW Yellow, NS Red         (3 s)   -> PED_COUNTDOWN or NS_GREEN
 *   PED_COUNTDOWN - NS Green,  EW Red + 7-seg (10 s)  -> NS_GREEN
 *   NS_GREEN      - NS Green,  EW Red         (6 s)   -> NS_YELLOW
 *   NS_YELLOW     - NS Yellow, EW Red         (3 s)   -> EW_GREEN
 *   ALL_RED_FLASH - All Reds flashing (safety)        -> EW_GREEN on S2 press
 */

#include <xc.h>
#include <stdint.h>
#include "Configuration.h"

#define _XTAL_FREQ 16000000UL

/* FSM State Definitions */
typedef enum {
    EW_GREEN,
    EW_YELLOW,
    NS_GREEN,
    NS_YELLOW,
    PED_COUNTDOWN,
    ALL_RED_FLASH
} TrafficState;

/* 7-Segment Patterns for PORTC (Common Anode: 0=ON, 1=OFF)
 *
 *   Bit:   7  6  5  4  3  2  1  0
 *   Seg:   g  f  -  e  d  c  b  a     (bit 5 = RC5 button, always mask it)
 *
 *        aaa
 *       f   b
 *        ggg
 *       e   c
 *        ddd
 */
static const uint8_t sevenSeg[10] = {
    0xDF, // TODO: 0 (a,b,c,d,e,f ON, g OFF)
    0xDF, // TODO: 1
    0xDF, // TODO: 2
    0xDF, // TODO: 3
    0xDF, // TODO: 4
    0xDF, // TODO: 5
    0xDF, // TODO: 6
    0xDF, // TODO: 7
    0xDF, // TODO: 8
    0xDF  // TODO: 9
};

#define SEG_BLANK 0xDF          /* All segments OFF (preserves RC5 bit) */

/* Pedestrian request flag (latched) */
static uint8_t pedRequest = 0;

static void init(void) {
    // TODO: Oscillator - 16 MHz HFINTOSC (IRCF/SCS)

    // TODO: Make PORTA/B/C/D/E digital (ANSELx = 0x00)

    // TODO: PORTD RD0-RD5 outputs, PORTB RB0-RB5 outputs (preserve bits 6-7)

    // TODO: PORTC - segments as outputs, RC5 as input (button S2)

    // TODO: RE0 input (pedestrian button), RA4 output (request LED)

    // TODO: Blank the 7-segment display
}

/** Update traffic lights, touching only bits 0-5 of each port. */
static void set_lights(uint8_t rd_val, uint8_t rb_val) {
    // TODO: same masked write as the base lab
}

/** Show one digit (0-9) on the 7-segment display, preserving RC5. */
static void seg_show(uint8_t digit) {
    // TODO: LATC = (LATC & 0x20) | (sevenSeg[digit] & ~0x20)
}

/**
 * Delay in ~10 ms slices while polling the buttons, so a press is never
 * missed during a long state delay.
 * Returns early if the safety button (S2) is pressed.
 */
static uint8_t delay_and_poll(uint16_t ms) {
    // TODO: loop in small __delay_ms(10) steps:
    //   - if RE0 pressed -> latch pedRequest = 1 and light RA4
    //   - if RC5 (S2) pressed -> return 1 immediately (enter safety mode)
    return 0;
}

void main(void) {
    init();

    TrafficState state = EW_GREEN;

    while (1) {
        // TODO: Implement the FSM with a switch(state):
        //
        //   EW_GREEN:      lights, delay_and_poll(6000), -> EW_YELLOW
        //   EW_YELLOW:     lights, delay_and_poll(3000),
        //                  -> PED_COUNTDOWN if pedRequest else NS_GREEN
        //   PED_COUNTDOWN: NS green / EW red; count 9..0 on the 7-seg,
        //                  1 second per digit; clear pedRequest and RA4;
        //                  blank display; -> NS_GREEN
        //   NS_GREEN:      lights, delay_and_poll(6000), -> NS_YELLOW
        //   NS_YELLOW:     lights, delay_and_poll(3000), -> EW_GREEN
        //   ALL_RED_FLASH: flash both red pairs at ~1 Hz until S2 is
        //                  pressed again, then -> EW_GREEN
        //
        // Any state: if delay_and_poll() reports S2, go to ALL_RED_FLASH.
        (void)state;
    }
}
