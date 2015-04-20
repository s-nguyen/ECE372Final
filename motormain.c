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
    forward, idle, backward, keepRunning, turnAround, rightTurn
} stateType;

volatile stateType curState = forward;
volatile stateType nextState;

volatile int timerCount = 0;
volatile int adcVal1, adcVal2, adcVal3, adcVal4 = 0;

int main(void)
{
    
    initPWM();
    initADC();
    initSW();
    initTimer1();
    //Enable ON the H-bridge
    ENABLEPIN = OUTPUT;
    ENABLE = 1;

    while(1){
        RIGHTWHEEL = 500;
        LEFTWHEEL = 500;
       /* switch(curState){
            case forward:
                //Change direct here
                PIN6 = 0;
                PIN4 = 0;
                PIN5 = 19;
                PIN7 = 18;
                curState = keepRunning;
                //curState = keepRunning;
                break;
            case keepRunning:
                //T1CONbits.TON = 1;
                RIGHTWHEEL = 750;
                LEFTWHEEL = 750;
                nextState = idle;

//                if (timerCount == 200){
//                    T1CONbits.TON = 0;
//                    TMR1 = 0;
//                    timerCount = 0;
//                    curState = idle;
//                }
                break;
            case idle:
                //Do nothing State
                LEFTWHEEL = 0;
                RIGHTWHEEL = 0;

                if (adcVal2 < 600 && adcVal3 < 600)
                    nextState = forward;
                break;
        }/*
            case backward:
                //Change direction here
                PIN5 = 0; //0 for NULL not used
                PIN7 = 0; 
                PIN6 = 18; //Pin 6 is maped to OC1 control left wheel
                PIN4 = 19; //Pin 4 is mapped to OC2 Control Right wheel
                curState = keepRunning;
                break;
            case turnAround:
                PIN4 = 19;
                PIN5 = 0;
                PIN6 = 0;
                PIN7 = 18;
                RIGHTWHEEL = 500;
                LEFTWHEEL = 500;
                T1CONbits.TON = 1;

                if (timerCount == 10){
                    T1CONbits.TON = 0;
                    timerCount = 0;
                    curState = idle; //just for testing: should be forward
                }
                break;
            case rightTurn:
                RIGHTWHEEL = 0;
                LEFTWHEEL = 1023;
                T1CONbits.TON = 1;

                if (timerCount == 5){
                    T1CONbits.TON = 0;
                    timerCount = 0;
                    curState = idle; //just for testing: should be forward
                }
                break;
            case idle:
                //Do nothing State
                LEFTWHEEL = 0;
                RIGHTWHEEL = 0;
                nextState = forward;
                break;
            case keepRunning:
                //All Detect
                if (adcVal1 < 650 && adcVal2 < 650 && adcVal3 < 650 && adcVal4 < 650){
                    curState = turnAround;
                }
                //Middle Two are On Line
                else if (adcVal2 < 650 && adcVal3 < 650){
                    //Go Striaght
                    RIGHTWHEEL = 1023;
                    LEFTWHEEL = 1023;
                }
                //2 Detects but 3 doesn't
                else if (adcVal2 < 650 && adcVal3 > 700){
                    RIGHTWHEEL = 800;
                    LEFTWHEEL = 1023;
                }
                //3 Detects but 2 doesn't
                else if (adcVal3 < 650 && adcVal2 > 700){
                    RIGHTWHEEL = 1023;
                    LEFTWHEEL = 800;
                }
                else if (adcVal1 < 650){
                    //ignore Left turn for now
                }
                else if (adcVal4 < 650){
                    curState = rightTurn;
                }
                else{
                    curState = backward; //Default to go backwards to find line
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

// ******************************************************************************************* //

void _ISR _T1Interrupt(void){
    //Put down the timer 1 flag first!
    IFS0bits.T1IF = 0;
    timerCount = timerCount + 1;

}