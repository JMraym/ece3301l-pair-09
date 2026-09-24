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

    /* 7-Segment Patterns for PORTC (Common Cathode: 0=OFF, 1=ON)
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
        0x5F, // TODO: 0 (a,b,c,d,e,f ON, g OFF)
        0x06, // TODO: 1
        0xBB, // TODO: 2
        0xAF, // TODO: 3
        0xE6, // TODO: 4
        0xED, // TODO: 5
        0xFD, // TODO: 6
        0x27, // TODO: 7
        0xFF, // TODO: 8
        0xE7  // TODO: 9
    };

    #define SEG_BLANK 0x20          /* All segments OFF (preserves RC5 bit) */

    /* Pedestrian request flag (latched) */
    static uint8_t pedRequest = 0;

    static void init(void) {
        // TODO: Oscillator - 16 MHz HFINTOSC (IRCF/SCS)
        OSCCONbits.IRCF = 0b111;
        OSCCONbits.SCS = 0b10;

        // TODO: Make PORTA/B/C/D/E digital (ANSELx = 0x00)
        ANSELA = 0x00;
        ANSELB = 0x00;
        ANSELC = 0x00;
        ANSELD = 0x00;
        ANSELE = 0x00;

        // TODO: PORTD RD0-RD5 outputs, PORTB RB0-RB5 outputs (preserve bits 6-7)
        TRISD &= 0xC0;
        TRISB &= 0xC0;

        // TODO: PORTC - segments as outputs, RC5 as input (button S2)
        TRISC &= 0x20;

        // TODO: RE0 input (pedestrian button), RA4 output (request LED)
        TRISEbits.TRISE0 = 1;
        TRISAbits.TRISA4 = 0;

        // TODO: Blank the 7-segment display
        LATC &= SEG_BLANK;
    }

    /** Update traffic lights, touching only bits 0-5 of each port. */
    static void set_lights(uint8_t rd_val, uint8_t rb_val) {
        // TODO: same masked write as the base lab
        LATD = (LATD & 0xC0) | (rd_val & 0x3F);
        LATB = (LATB & 0xC0) | (rb_val & 0x3F);
    }

    /** Show one digit (0-9) on the 7-segment display, preserving RC5. */
    static void seg_show(uint8_t digit) {
        // TODO: LATC = (LATC & 0x20) | (sevenSeg[digit] & ~0x20)
        LATC = (LATC & 0x20) | (sevenSeg[digit] & ~0x20);
    }

    /**
     * Delay in ~10 ms slices while polling the buttons, so a press is never
     * missed during a long state delay.
     * Returns early if the safety button (S2) is pressed.
     */
    static uint8_t delay_and_poll(uint16_t ms) {
        // TODO: loop in small __delay_ms(10) steps:
        for (uint16_t c = 0; c < ms ; c += 10){
            __delay_ms(10);

        //   - if RE0 pressed -> latch pedRequest = 1 and light RA4
            if (PORTEbits.RE0 == 1){
                pedRequest = 1;
                LATAbits.LATA4 = 1;
            }
        //   - if RC5 (S2) pressed -> return 1 immediately (enter safety mode)
            if (PORTCbits.RC5 == 0){
                __delay_ms(50); 
                 // Wait for S2 to be released
                while (PORTCbits.RC5 == 0);
                __delay_ms(50);
                return 1;
            }
        }
        return 0;
    }

    void main(void) {
        init();
        
        TrafficState state = EW_GREEN;

        while (1) {
            switch(state){

            // TODO: Implement the FSM with a switch(state):
            //
            //   EW_GREEN:      lights, delay_and_poll(6000), -> EW_YELLOW
                case EW_GREEN:
                    set_lights(0x09, 0x24);
                    if (delay_and_poll(6000)) {
                        state = ALL_RED_FLASH;
                    } else {
                        state = EW_YELLOW;
                    }
                    break;

            //   EW_YELLOW:     lights, delay_and_poll(3000),
            //                  -> PED_COUNTDOWN if pedRequest else NS_GREEN
                case EW_YELLOW:
                    set_lights(0x09, 0x12); // NS Red, EW Yellow
                    if (delay_and_poll(3000)) {
                        state = ALL_RED_FLASH;
                    } else {
                        if (pedRequest == 1) {
                            state = PED_COUNTDOWN;
                        } else {
                            state = NS_GREEN;
                        }
                    }
                    break;

            //   PED_COUNTDOWN: NS green / EW red; count 9..0 on the 7-seg,
            //                  1 second per digit; clear pedRequest and RA4;
                case PED_COUNTDOWN:
                    set_lights(0x24, 0x09); // NS Green, EW Red
                    
                    for (int8_t j = 9; j >= 0; j--) {
                        seg_show((uint8_t)j);
                        if (delay_and_poll(1000)) {
                            state = ALL_RED_FLASH;
                            break;
                        }
                    }
                    
                   
                //                  blank display; -> NS_GREEN
                    LATC &= SEG_BLANK;
                    
                //clear ped request and A4 after countdown
                    pedRequest = 0;
                    LATAbits.LATA4 = 0;
                        
                    if (state != ALL_RED_FLASH) {
                        state = NS_GREEN;
                    }
                    break;

            //   NS_GREEN:      lights, delay_and_poll(6000), -> NS_YELLOW
                case NS_GREEN:
                    set_lights(0x24, 0x09);
                    if (delay_and_poll(6000)) {
                        state = ALL_RED_FLASH;
                    } else {
                    state = NS_YELLOW;
                    }
                    break;

            //   NS_YELLOW:     lights, delay_and_poll(3000), -> EW_GREEN
                case NS_YELLOW:
                    set_lights(0x12,0x09);
                    if (delay_and_poll(3000)) {
                        state = ALL_RED_FLASH;
                    } else {
                    state = EW_GREEN;
                    }
                    break;
            //   ALL_RED_FLASH: flash both red pairs at ~1 Hz until S2 is
            //                  pressed again, then -> EW_GREEN
                case ALL_RED_FLASH:
                    LATC &= SEG_BLANK;
                    while (PORTCbits.RC5 != 0){
                        set_lights(0x09,0x09);
                        __delay_ms(500);
                        set_lights(0x00,0x00);
                        __delay_ms(500);
                    }
                    __delay_ms(50);
                    while (PORTCbits.RC5 == 0); // Wait for button release
                    __delay_ms(50);
                    
                    state = EW_GREEN;
                    break;
            // Any state: if delay_and_poll() reports S2, go to ALL_RED_FLASH.

            (void)state;
            }
        }
    }
