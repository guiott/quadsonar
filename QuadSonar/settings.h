/*
 * File:   settings.h
 * Author: guiott
 *
 * Created on 29 febbraio 2012, 20.29
 */

#ifndef settings_H
#define settings_H
    extern int Timer0_150us;
    extern int Timer0_850us;
    extern int Timer0_49ms;
    extern float CmTick;

    extern struct Bits
    {
            unsigned bit0:1;
            unsigned bit1:1;
            unsigned bit2:1;
            unsigned bit3:1;
            unsigned bit4:1;
            unsigned bit5:1;
            unsigned bit6:1;
            unsigned bit7:1;
    };
    extern struct Bits VARbits1;
#endif
