/*
 * File:   definition.h
 * Author: guiott
 *
 * Created on 1 marzo 2012, 10.16
 */

#ifndef definition_H
#define definition_H

struct Bits
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

struct Bits VARbits1;

//----- Variables
int i = 0; // contatore generico
int j = 0; // contatore generico
char TimerStatus = 0;

unsigned int Timer0_150us;
unsigned int Timer0_850us;
unsigned int Timer0_49ms;
unsigned int Dist[4];
unsigned char UsIndx = 0;

unsigned char Ccpr2HBuff;   // a further buffer to avoid TMR value overwrite
unsigned char Ccpr2LBuff;

unsigned char TxBuff[32];
unsigned char TxBuffLen;
unsigned char TxBuffIndx;

//How many cm for each TIMER1 tick / 2
//because the echo travels forth and back to the object
float CmTick;

#define MUX_DISABLE PORTBbits.RB3       //Multiplexer disable
#define MUX_SELECT_LOW PORTBbits.RB4    //Multiplexer select MSB
#define MUX_SELECT_HIGH PORTBbits.RB5   //Multiplexer select LSB

//I2C definitions
unsigned char I2cRegPtr;//Pointer to first byte to read or write in the register

//TX registers array
#define I2C_BUFF_SIZE_TX 11//7 for the corrected measures + 4 for the raw values
unsigned char I2cRegTx[I2C_BUFF_SIZE_TX]={0,0,0,0,0,0,0,0,0,0,0};//TX registers array

#define I2C_BUFF_SIZE_RX 7

unsigned char I2cRegRx[I2C_BUFF_SIZE_RX]={0,0,0,0,0,0,0};

#endif
