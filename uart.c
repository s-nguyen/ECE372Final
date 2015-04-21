#include "p24FJ64GA002.h"
#include "uart.h"

// ******************************************************************************************* //
// Defines to simply UART's baud rate generator (BRG) regiser
// given the osicllator freqeuncy and PLLMODE.

#define XTFREQ          7372800         	  // On-board Crystal frequency
#define PLLMODE         4               	  // On-chip PLL setting (Fosc)
#define FCY             (XTFREQ*PLLMODE)/2    // Instruction Cycle Frequency (Fosc/2)

#define BAUDRATE        9600
#define BRGVAL          ((FCY/BAUDRATE)/16)-1

void initUART(){
    RPINR18bits.U1RXR = 9;
    RPOR4bits.RP8R = 3;
    U1BRG  = BRGVAL;
    U1MODE = 0x8000;
    U1STA  = 0x0440; 		// Reset status register and enable TX & RX
    IFS0bits.U1RXIF = 0;
}
