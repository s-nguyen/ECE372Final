#include "p24FJ64GA002.h"
#include "adc.h"

void initADC(){
    AD1PCFGbits.PCFG9 = 0; //Set AN9 to analog
    AD1PCFGbits.PCFG10 = 0; //Set AN10 to analog
    AD1PCFGbits.PCFG11 = 0; //Set AN11 to analog
    AD1PCFGbits.PCFG12 = 0; //Set AN12 to analog
    
    AD1CON1bits.FORM = 0;
    AD1CON1bits.SSRC = 7;
    AD1CON1bits.ASAM = 1;
    AD1CON2bits.VCFG = 0;
    AD1CON2bits.CSCNA = 1; //Enable Scanning
    AD1CON2bits.SMPI = 3; //Amount of buffers
    AD1CON2bits.ALTS = 0;
    AD1CON3bits.ADRC = 0;
    AD1CON3bits.SAMC = 1;
    AD1CON3bits.ADCS = 10;
    AD1CHS = 0; //Configure input channels,
    //AD1CHSbits.CH0SA = 0000; //Select Pins
    AD1CSSL = 0;
    AD1CSSLbits.CSSL9 = 1; //AN9 scan
    AD1CSSLbits.CSSL10 = 1; //AN 10 scan
    AD1CSSLbits.CSSL11 = 1; //AN 11 scan
    AD1CSSLbits.CSSL12 = 1; //AN 12 scan
    AD1CON1bits.ADON = 1; // Turn on A/D
    IEC0bits.AD1IE = 1; //Enable the interrupt
    IFS0bits.AD1IF = 0; //Put the flag down
    //Interupt will be called as soon as it fill up one spot in the buffer
}
