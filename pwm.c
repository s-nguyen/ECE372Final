#include "p24FJ64GA002.h"
#include "pwm.h"


void initPWM(){

    T3CONbits.TCKPS = 0; //Fixed
    TMR3 = 0;
    PR3 = 1023; //Timing is not important?
    

    OC1CONbits.OCTSEL = 1; //Using Timer 3
    OC2CONbits.OCTSEL = 1;

    OC1CONbits.OCM = 6; //PWM mode on OCx,Fault pin disabled
    OC2CONbits.OCM = 6;
    
    PIN6 = 18; //Pin 6 is maped to OC1 control left wheel
    PIN4 = 19; //Pin 4 is mapped to OC2 Control Right wheel
    
    OC1R = 0;
    OC2R = 0;
    
    OC1RS = 0; //Start off zero duty cycle
    OC2RS = 0;

    T3CONbits.TON = 1; //Turn timer 3 on at the end
}