/* 
 * File:   ch_blink.c
 * Author: Kim Chesed Paller
 *
 * Created on May 21, 2014, 4:15 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include "config.h"

/*
 * 
 */

/*----------------------------------------------------------------------*-
 * Setup the system clock. FCY = 40 Mhz , FOSC = 80 Mhz
 -*----------------------------------------------------------------------*/
void SetupClock(){
    // Configure PLL prescaler, PLL postscaler, PLL divisor
    PLLFBD=38; // M = 40
    CLKDIVbits.PLLPOST = 0; // N2 = 2
    CLKDIVbits.PLLPRE = 0; // N1 = 2

    // Initiate Clock Switch to Internal FRC with PLL (NOSC = 0b011)
    __builtin_write_OSCCONH(0x03);
    __builtin_write_OSCCONL(0x01);

    // Wait for Clock switch to occur
    while (OSCCONbits.COSC != 0b011){};
    // Wait for PLL to lock
    while(OSCCONbits.LOCK != 1) {};
}

/*----------------------------------------------------------------------*-
 * Initialize UART1 @ 9600 bps
 -*----------------------------------------------------------------------*/
void InitUART1(){

    //configure U1Mode Register for 8-2-1 operation
    U1MODEbits.UARTEN = 0;	// Bit15 TX, RX DISABLED, ENABLE at end of func
    //U2MODEbits.notimplemented;// Bit14
    U1MODEbits.USIDL = 0;	// Bit13 Continue in Idle
    U1MODEbits.IREN = 0;	// Bit12 No IR translation
    U1MODEbits.RTSMD = 0;	// Bit11 Flow Control Mode
    //U2MODEbits.notimplemented;	// Bit10
    U1MODEbits.UEN = 0;		// Bits8,9 TX,RX enabled, CTS,RTS not
    U1MODEbits.WAKE = 0;	// Bit7 No Wake up (since we don't sleep here)
    U1MODEbits.LPBACK = 0;	// Bit6 No Loop Back
    U1MODEbits.ABAUD = 0;	// Bit5 No Autobaud
    U1MODEbits.URXINV = 0;	// Bit4 IdleState = 1  (for dsPIC)
    U1MODEbits.BRGH = 0;	// Bit3 16 clocks per bit period, Standard Speed Mode
    U1MODEbits.PDSEL = 0;	// Bits1,2 8bit, No Parity
    U1MODEbits.STSEL = 0;	// Bit0 One Stop Bit

    // Load a value into Baud Rate Generator.  Example is for 9600.
    U1BRG = 260;

    // Load all values in for U1STA SFR

    //Uart interrupt Configurations
    //U1STAbits.UTXISEL1 = 0;	//Bit15 Int when a word is transfered and  transmit has atleast 1 empty room
    //U1STAbits.UTXISEL0 = 0;	//Bit13 Other half of Bit15
    //U1STAbits.URXISEL = 0b10;     //Interrupt when  receive buffer contains three or four characters.


    //IFS0bits.U1TXIF = 0;	// Clear the Transmit Interrupt Flag
    //IEC0bits.U1TXIE = 1;      // Enable UART TX interrupt

    //IEC0bits.U1RXIE = 1;        //Enable UART RX interrupt
    U1MODEbits.UARTEN = 1;      // Enable UART
    U1STAbits.UTXEN = 1;        // Enable UART TX

    //__delay_ms(1000);

}

int main(int argc, char** argv) {

    return (EXIT_SUCCESS);
}

