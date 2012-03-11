/* //////////////////////////////////////////////////////////////////////////////////////
** File:      QuadSonar.c
*/                                  
#define  Ver "QuadSonar v.0.10\r       by Guiott"

/**
* \mainpage QuadSonar.c
* \author    Guido Ottaviani-->guido@guiott.com<--
* \version 0.0.1
* \date 03/12
* \details This software drives a chinese parking sensors instead of the
 * original 8051 based controller to achieve a waterproof, quadruple
 * Sonar Range Finder.
 * All the rest of the hardware remain the same.
 * In this way it is possible to make any customization:
 * - changing finding range
 * - changing scanning period
 * - using different communication channels (e.g.: I2c or serial)
 *
-------------------------------------------------------------------------------
\copyright 2011 Guido Ottaviani
guido@guiott.com

    QuadSonar is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    QuadSonar is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with dsPID33.  If not, see <http://www.gnu.org/licenses/>.

-------------------------------------------------------------------------------
*/

// include standard
#include <stdlib.h>
#include <math.h>
#include <pwm.h>
#include <i2c.h>

#include "common.h"
#include "definition.h"
#include "prototypes.h"
#include "statusBits.h"


void main (void)	// ==========
{	
  
INTCONbits.GIEH=0;	// disable interrupt high
INTCONbits.GIEL=0;	// disable interrupt low


Settings(); // imposta porte e registri (settings.h)



// ==== enable interrupts
INTCONbits.GIEH=1;	// enable interrupt high
INTCONbits.GIEL=1;	// enable interrupt low

SetTimer0(Timer0_49ms);
T0CONbits.TMR0ON=1;     //TMR0 on
SetMux(UsIndx);
TX_FLAG=0;

while (1)  // main loop
{	
    if(TIMER0_FLAG)
    {
        Sonar();
    }

    if(CAPTURE_FLAG)
    {
        Echo();
    }

    if(TX_FLAG)
    {// there is a string to transmit
        if(PIR1bits.TXIF)
        {// the TX buffer TXREG is empty
            TxChar();
        }
    }

}  // main loop
} // main ==========



/* Functions *****************************************************************/
void TxChar(void)
{/**
 *\brief load TXREG with the next byte to send
  *
 */
    // to be done ??????????????????????????????????????????????
    // at the end of the buffer reset TX_FLAG
}

void SetTxBuffer(void)
{/**
 *\brief fill the TX buffer with the ASCII values
  *
 */
    // to be done  ?????????????????????????????????
    TX_FLAG=1;
}

void Echo(void)
{/**
 *\brief an echo returned. Read the capture registers to compute echo time
 *
 */
    CAPTURE_FLAG=0;
    ECHO_FLAG = 1; // at least one echo arrived
    // Timer1 * HowManyCmPerEachTmr1Tick = Distance from 0 to 255 cm
    Dist[UsIndx] = ((Ccpr2HBuff<<8) + Ccpr2LBuff) * CmTick;
    SelectUS();
}

void SelectUS(void)
{/**
 *\brief set the UsIndx in order to cycle among the four transducers
  * US Left Left:   UsIndx = 0b01
  * US Left Center: UsIndx = 0b11
  * US Right Center:UsIndx = 0b10
  * US Right Right: UsIndx = 0b00
 */
    UsIndx++;
    if (UsIndx>3)
    {
        UsIndx=0;
        // a full cycle is over. The values can be transmitted
        SetTxBuffer();
    }
    
    SetMux(UsIndx);
}

void SetMux(int MuxPort)
{
/**
 *\brief Set the selected port on analog multiplexer
 *
*/
    MUX_DISABLE = 0;
    MUX_SELECT_LOW  = MuxPort;
    MUX_SELECT_HIGH = MuxPort >> 1;
}

void Sonar()
{/**
 *\brief set the sequence for 40KHz burst and perform the operation to measure
  *      the echo flying time
 *
 */
       TIMER0_FLAG=0;
        switch (TimerStatus)
        {
            case 0:
                PIE2bits.CCP2IE=0; //CCP2 int enabled only when needed
                if (ECHO_FLAG==0)
                {
                    Dist[UsIndx]=255; // if no echo back Dist = maximum
                    SelectUS();
                }
                else
                {
                    ECHO_FLAG = 0;
                }
                PIE2bits.CCP2IE=0;        // CCP2 int enabled only when needed
                ResetTimer1(); // reset the timer to measure the eco delay
                SetTimer0(Timer0_150us);  // 40KHz burst duration
                SetPWM(1);                // switch on the 40KHz pulse
                break;

            case 1:
                SetTimer0(Timer0_850us); // wait for the pulse decay time
                SetPWM(0);               // switch off the 40KHz pulse
                break;

            case 2:
                PIR2bits.CCP2IF = 0;      // reset of CCP2 int flag
                PIE2bits.CCP2IE=1;        // CCP2 int enabled only when needed
                SetTimer0(Timer0_49ms);   // wait for the echo before
                                          // starting a new pulse
                SetPWM(0);
                break;

            default:
                SetTimer0(Timer0_49ms);
                SetPWM(0);
                break;
        }

        TimerStatus ++;
        if(TimerStatus > 2)
        {
            TimerStatus=0;
        }
}

void SetTimer0(int Count)
{
/**
 *\brief set both High and Low registers for Timer0
 *
 */

TMR0H = Count >> 8; // byte High
TMR0L = Count;      // byte Low
}

void ResetTimer1(void)
{
/**
 *\brief Timer1 start from 0
 *
 */

TMR1H = 0; // byte High
TMR1L = 0; // byte Low
}

void SetPWM(int On)
{
    if(On==0)
    {
        SetDCPWM1(0);  // duty cycle 0%
    }
    else
    {
        SetDCPWM1(500); // duty cycle 50%
    }
}

/*===========================================================================*/

// Low priority interrupt vector

#pragma code LowVector = 0x18
void InterruptVectorLow (void)
{
  _asm
    goto InterruptHandlerLow //jump to interrupt routine
  _endasm
}

//----------------------------------------------------------------------------
// Low priority interrupt routine

#pragma code
#pragma interruptlow InterruptHandlerLow

/*===========================================================================*/

/*IntServiceRoutine***********************************************************/
void InterruptHandlerLow (void)

{  
 if (PIR2bits.CCP2IF)     // CCP2 capture interrupt occurred
 {
    Ccpr2HBuff=CCPR2H;  // to avoid overwrite
    Ccpr2LBuff=CCPR2L;

    CAPTURE_FLAG = 1;
    PIR2bits.CCP2IF = 0;// reset of interrupt flag

    // only the first echo is captured.
    PIE2bits.CCP2IE=0;//The interrupt will be enabled again at the next cycle
 }
}   // Low Priority IntServiceRoutine
/*****************************************************************************/


/*===========================================================================*/
// High priority interrupt vector

#pragma code HighVector = 0x08
void
InterruptVectorHigh (void)
{
  _asm
    goto InterruptHandlerHigh //jump to interrupt routine
  _endasm
}

//----------------------------------------------------------------------------
// High priority interrupt routine

#pragma code
#pragma interrupt InterruptHandlerHigh

/*===========================================================================*/


/*IntServiceRoutine***********************************************************/
void InterruptHandlerHigh (void)

{      
 if (INTCONbits.TMR0IF)   // timer 0 overflow?
 {
    TIMER0_FLAG = 1;
    INTCONbits.TMR0IF = 0;// reset of interrupt flag
 }
}   // High Priority IntServiceRoutine

