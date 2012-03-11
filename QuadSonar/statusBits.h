/*
 * File:   config.h
 * Author: guiott
 *
 * Created on 1 marzo 2012, 10.20
 */

#ifndef config_H
#define config_H

//----- Status bits
#ifdef PLL  // 40MHz
	#pragma config OSC = HSPLL  //	10 x 4 = 40MHZ
#else
	#pragma config OSC = HS     //	20Mhz
#endif

#pragma config WDT = OFF
#pragma config LVP = OFF
#pragma config BOREN = OFF
#pragma config CCP2MX = PORTC   // CCP2 capture on pin RC1

//#pragma config PWRT = ON not compatible with ICD2
//#pragma config XINST = ON

#endif
