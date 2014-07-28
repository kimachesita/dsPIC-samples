/* 
 * File:   tt_blink.c
 * Author: Kim Chesed Paller
 *
 * Created on May 19, 2014, 10:47 AM
 */

#include <stdio.h>
#include <stdlib.h>
#include "config.h"
#include <libpic30.h>

#define FCY 40000000ULL //Fcy = (Fosc/2) Fosc = 80MHz
/*
 * 
 */

/*Initialization Routines*/
void SetupClock(void){
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
void InitLED(void){
    TRISAbits.TRISA0 = 0;
    LATAbits.LATA0 = 0;
}
void InitT89(void){

    T9CONbits.TON = 0; // Stop any 16-bit Timer9 operation
    T8CONbits.TON = 0; // Stop any 16/32-bit Timer8 operation

    //Timer Mode Configuration @32-bit
    T8CONbits.T32 = 1; // Enable 32-bit Timer mode
    T8CONbits.TCS = 0; // Select internal instruction cycle clock
    T8CONbits.TGATE = 0; // Disable Gated Timer mode
    //T8CONbits.TSYNC = 0;

    //Timer 1 Prescaler configuration 1:256
    T8CONbits.TCKPS = 0b11;

    TMR9 = 0x00; // Clear 32-bit Timer (msw)
    TMR8 = 0x00; // Clear 32-bit Timer (lsw)
    PR9 = 0x0002; // Load 32-bit period value (msw)
    PR8 = 0x625A; // Load 32-bit period value (lsw)

    IPC13bits.T9IP = 0x01; // Set Timer3 Interrupt Priority Level
    IFS3bits.T9IF = 0; // Clear Timer3 Interrupt Flag
    IEC3bits.T9IE = 1; // Enable Timer3 interrupt

    T8CONbits.TON = 1; // Start 32-bit Timer

}

/*ISRs*/
void __attribute__((__interrupt__, no_auto_psv)) _T9Interrupt(void){
/* Interrupt Service Routine code goes here */
    LATAbits.LATA0 = !LATAbits.LATA0;
    IFS3bits.T9IF = 0; // Clear Timer3 Interrupt Flag
}

/*Main Routine*/
int main(int argc, char** argv) {
    SetupClock();
    InitLED();
    InitT89();
    while(1);
    return (EXIT_SUCCESS);
}

