// ******************************************************************************************* //
//Stephen Nguyen & Brett Spradling
// File: 
// Date: 
// Authors: Stephen Nguyen
// Description: Move robot
// ******************************************************************************************* //
#include "p24fj64ga002.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "adc.h"
#include "pwm.h"
#include "timer.h"
#include "VCNL4000.h"
#include "I2C.h"
#include "supportFunctions.h"

#define PROXIMITY_THRESHOLD 2270

_CONFIG1( JTAGEN_OFF & GCP_OFF & GWRP_OFF & BKBUG_ON & COE_ON & ICS_PGx1 &
    FWDTEN_OFF & WINDIS_OFF & FWPSA_PR128 & WDTPS_PS32768 )
_CONFIG2( IESO_OFF & SOSCSEL_SOSC & WUTSEL_LEG & FNOSC_PRIPLL & FCKSM_CSDCMD & OSCIOFNC_OFF &
    IOL1WAY_OFF & I2C1SEL_PRI & POSCMOD_XT )
// ******************************************************************************************* //
// ******************************************************************************************* //
typedef enum stateTypeEnum{
    forward, changeTires, idle,
            scan, check, moveForward, moveRight, moveLeft, findLine, turnAround, goAround
} stateType;

volatile stateType curState = idle;
volatile stateType nextState;
volatile stateType prevState;

volatile int timerCount = 0;
volatile int adcVal1, adcVal2, adcVal3, adcVal4 = 0;

int main(void)
{
    int delay = 0;
    int f; 
    int goAroundCount = 0;
    unsigned int test = 0;
    int didTurnAround = 0;
    initPWM();
    initADC();
    initSW();
    initI2C();
    initVNCL4000();

    //Enable ON the H-bridge
    ENABLEPIN = OUTPUT;
    ENABLE = 1;
    TRISBbits.TRISB8 = 0;
    TRISBbits.TRISB9 = 0;
    while(1){
      
        switch(curState){
            case idle:
                //Do nothing State
                LEFTWHEEL = 0;
                RIGHTWHEEL =0;
                nextState = forward;
                break;
            case forward:
                //Change direct here
                PIN6 = 0; //pIn 5 and 7 is forward, Pin4 and 6 is backwards
                PIN4 = 0;
                PIN5 = 19;
                PIN7 = 18;
                prevState = forward;
                curState = scan;
                //curState = keepRunning;
                break;
            case changeTires:
                PIN4 = 0;
                PIN5 = 19;
                PIN6 = 18;
                PIN7 = 0;
                curState = turnAround;
                break;
            case scan:
                //All Detect
                AD1CON1bits.SAMP = 1;
                if(AD1CON1bits.DONE == 1){
                    curState = check;
                }
                break;
            case check:
                test = readProximity();

                if (test >= PROXIMITY_THRESHOLD){
                  curState = goAround;
                }

                else if ((adcVal1 <= 600 && adcVal2 <= 600 && adcVal3 <= 600 && adcVal4 <= 600)){ //All detecting
                    delay = 1;
                    if(didTurnAround == 0){
                        didTurnAround = 1;
                        curState = changeTires;
                    }
                    else{
                        curState = findLine;
                    }
                }

                else if(adcVal2 <= 600 && adcVal3 <= 600){
                    curState = moveForward;
                }
                else if(adcVal2 > 600 && adcVal3 <=600){
                    curState = moveRight;
                }
                else if(adcVal3 > 600 && adcVal2 <= 600){
                    curState = moveLeft;
                }
                else {
                    curState = findLine;
                }
                break;
            case moveForward:
                RIGHTWHEEL = 600; //1000;
                LEFTWHEEL = 600; //1000;
                curState = forward;
                break;
            case moveRight:
                RIGHTWHEEL = 0; //500
                LEFTWHEEL = 600;  //1000
                curState = forward;
                break;
            case moveLeft:
                RIGHTWHEEL = 600; //800
                LEFTWHEEL = 0;  //500
                curState = forward;
                break;
            case turnAround:
                RIGHTWHEEL = 600; //1000
                LEFTWHEEL = 600;
                //Can add in function
                if(delay == 1){
                    delayS(1);
                    delay = 0;
                }
                AD1CON1bits.SAMP = 1;
                if(AD1CON1bits.DONE == 1){
                    if(adcVal2 <= 600 && adcVal3 <= 600 && adcVal1 > 600 && adcVal4 > 600){
                    curState = forward;
                    }
                }
                
                break;
            case goAround:
                if (goAroundCount == 0){
                    LEFTWHEEL = 500;
                    RIGHTWHEEL = 800;
                    delayMs(500);
                    goAroundCount = 1;
                }
                else{
                    LEFTWHEEL = 800;
                    RIGHTWHEEL = 500;
                }
                AD1CON1bits.SAMP = 1;
                if(AD1CON1bits.DONE == 1){
                    if (adcVal2 <= 600 && adcVal3 <= 600){
                        curState = findLine;
                    }
                    else {
                        curState = goAround;
                    }
                }

                break;
                
            case findLine:
                goAroundCount = 0;
                 AD1CON1bits.SAMP = 1;
                 if(AD1CON1bits.DONE == 1){
                    if(adcVal2 <= 600 && adcVal3 <= 600){
                    curState = forward;
                    }
                    else if(adcVal1 <= 600 && adcVal4 > 600){
                        LEFTWHEEL = 0;
                        RIGHTWHEEL = 700;
                    }
                    else if(adcVal4 <= 600 && adcVal1 > 600){
                        RIGHTWHEEL = 0;
                        LEFTWHEEL = 700;
                    }
                    else if(adcVal1 <= 600 && adcVal4 <= 600){
                        LEFTWHEEL = 1000;
                        RIGHTWHEEL = 0;
                    }
                    else{
                        RIGHTWHEEL = 700;
                        LEFTWHEEL = 700;
                    }
                }   
                break;
            default:
                curState = idle;
                break;
        }
    }
    return 0;
}
// ******************************************************************************************* //
void _ISR _ADC1Interrupt(void){

    IFS0bits.AD1IF = 0;
    adcVal1 = ADC1BUF0;
    adcVal2 = ADC1BUF1;
    adcVal3 = ADC1BUF2;
    adcVal4 = ADC1BUF3;
    AD1CON1bits.SAMP = 0;
}
// ******************************************************************************************* //
void _ISR _CNInterrupt(void){
    IFS1bits.CNIF = 0;
    if(PORTBbits.RB5 == 1){
        curState = nextState;
    }
}

// ******************************************************************************************* //

