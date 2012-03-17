/*
 * File:   Common.h
 * Author: guiott
 *
 * Created on 28 febbraio 2012, 23.52
 */

#ifndef Common_H
    #define Common_H

    #include <p18f2620.h>

    #define PLL 1

    #ifdef PLL  // 40MHz
        #define XTAL_FREQ  40000000    //crystal frequency
        #define PRESCALER0 8            //TIMER0 selected prescaler
        #define PRESCALER1 8            //TIMER1 selected prescaler

    #else
        #define XTAL_FREQ  20000000    //crystal frequency
        #define PRESCALER0 4           //TIMER0 selected prescaler
        #define PRESCALER1 4           //TIMER1 selected prescaler
    #endif

    #define FCY   (XTAL_FREQ / 4)       //crystal divided by four
    #define TCY   1 / (float)FCY        //single instruction period

    /* sound speed variation with temperature
     T °C	 V m/s
     ?10	325,4
      ?5	328,5
       0	331,5
      +5	334,5
     +10	337,5
     +15	340,5
     +20	343,4
     +25	346,3
     +30	349,2
    */
    #define TMR1_TICK TCY*PRESCALER1    //TIMER1 tick duration
    #define SOUND_SPEED 340             //sound speed in m/s at 23*C

    //----- Flags
    #define TIMER0_FLAG VARbits1.bit0	// Timer Interrupt occurred
    #define CAPTURE_FLAG VARbits1.bit1  // Capture Interrupt occurred
    #define ECHO_FLAG VARbits1.bit2     // at least one echo arrived in a cycle
    #define TX_FLAG VARbits1.bit3       // it's time to transmit

    //----- Macro
    #define ABS(x) ((x) > 0 ? (x) : -(x)) // ABS() for int, long, float...

#endif
