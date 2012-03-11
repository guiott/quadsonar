/*
 * File:   prototypes.h
 * Author: guiott
 *
 * Created on 1 marzo 2012, 10.14
 */

#ifndef prototypes_H
#define prototypes_H

/*Prototypes*******************************************************************/

void Settings(void);
void InterruptHandlerHigh (void);
void InterruptHandlerLow (void);
void SetTimer0(int Count);
void SetPWM(int On);
void Cycle(void);
void ResetTimer1(void);
void SetMux(int MuxPort);
void Echo(void);
void SelectUS(void);
void SetTxBuffer(void);
void TxChar(void);
void Uchar2Ascii(unsigned char Indx);
#endif
