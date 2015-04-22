#include "adc.h"
#include "supportFunctions.h"
#include "p24FJ64GA002.h"
void checkSensor(){
    AD1CON1bits.SAMP = 1;
    
    while(AD1CON1bits.DONE == 0){

    }
}
