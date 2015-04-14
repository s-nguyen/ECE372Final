// ******************************************************************************************* //
//
// File: oScope.c
// Date: 10-1-2014
// Authors: Garrett Vanhoy
// Description: This shows the use of a PWM signal as well as initializing the ADC
// ******************************************************************************************* //
#include "p24fj64ga002.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "adc.h"
#include "pwm.h"
#include "timer.h"

_CONFIG1( JTAGEN_OFF & GCP_OFF & GWRP_OFF & BKBUG_ON & COE_ON & ICS_PGx1 &
    FWDTEN_OFF & WINDIS_OFF & FWPSA_PR128 & WDTPS_PS32768 )
_CONFIG2( IESO_OFF & SOSCSEL_SOSC & WUTSEL_LEG & FNOSC_PRIPLL & FCKSM_CSDCMD & OSCIOFNC_OFF &
    IOL1WAY_OFF & I2C1SEL_PRI & POSCMOD_XT )
// ******************************************************************************************* //
// ******************************************************************************************* //
typedef enum stateTypeEnum{
    forward, idle, backward, keepRunning
} stateType;

volatile stateType curState = idle;
volatile stateType nextState;

volatile int done = 0;
volatile int adcVal1, adcVal2, adcVal3, adcVal4 = 0;

int main(void)
{
    char str[9]; //Used to print to LCD
    double voltage;
    
    initPWM();
    initADC();
    initSW();
    //Enable ON the H-bridge
    ENABLEPIN = OUTPUT;
    ENABLE = 1;

    while(1){
        LEFTWHEEL = adcVal4;
        RIGHTWHEEL = adcVal4;
        /*switch(curState){
            case forward :
                //Change direct here
                PIN6 = 0;
                PIN4 = 0;
                PIN5 = 19;
                PIN7 = 18;
                nextState = idle;
                curState = keepRunning;
                break;
            case backward:
                //Change direction here
                PIN5 = 0; //0 for NULL not used
                PIN7 = 0; 
                PIN6 = 18; //Pin 6 is maped to OC1 control left wheel
                PIN4 = 19; //Pin 4 is mapped to OC2 Control Right wheel
                nextState = idle;
                curState = keepRunning;
                break;
            case idle:
                //Do nothing State
                LEFTWHEEL = 0;
                RIGHTWHEEL = 0;
                nextState = backward;
                break;
            case keepRunning:
                //All Detect
                if (adcVal1 < 300 && adcVal2 < 300 && adcVal3 < 300 && adcVal4 < 300){
                    //Figure out what to do. Could default to 180 for first iteration
                    //Need a counter to detect two in a row
                    //But if sampling is too fast it will detect the same band as 2-2923 in a row
                }
                //Middle Two are On Line
                else if (adcVal2 < 300 && adcVal3 < 300){
                    //Go Striaght
                    RIGHTWHEEL = 1023;
                    LEFTWHEEL = 1023;
                }
                //2 Detects but 3 doesn't
                else if (adcVal2 < 300 && adcVal3 > 700){
                    RIGHTWHEEL = 800;
                    LEFTWHEEL = 1023;
                }
                //3 Detects but 2 doesn't
                else if (adcVal3 < 300 && adcVal2 > 700){
                    RIGHTWHEEL = 1023;
                    LEFTWHEEL = 800;
                }
                else if (adcVal1 < 300){
                    //LEFT TURN
                }
                else if (adcVal4 < 300){
                    //RIGHT TURN
                }
                else{
                    //curState = backward; //Default to go backwards to find line
                    LEFTWHEEL = 1023;   //Middle should have both on max
                    RIGHTWHEEL = 1023;
                }
                break;
            default:
                curState = forward;
                break;
        }*/
    }
    return 0;
}
// ******************************************************************************************* //
void _ISR _ADC1Interrupt(void){

    IFS0bits.AD1IF = 0;
    adcVal1 = ADC1BUF0;        //Read from one buffer from ADC
    adcVal2 = ADC1BUF1;        //Read from ADC for 2nd transistor
    adcVal3 = ADC1BUF2;        //Read from ADC for 3rd transistor
    adcVal4 = ADC1BUF3;        //Read from ADC for 4th transistor
}
// ******************************************************************************************* //
void _ISR _CNInterrupt(void){
    IFS1bits.CNIF = 0;
    if(PORTBbits.RB5 == 1){
        curState = nextState;
    }

}