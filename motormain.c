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

//adcVal1 is the most left sensor
//adcVal2 is the middle left sensor which is always on the line
//adcVal3 is the midde right sensor which is always on the line
//adcVal4 is the most right sensor which is always on right of the black tape


int main(void)
{
    int delay = 0; //Delay Flag for the 180 turn. It will disable the scanner once when all the led read black
    int goAroundCount = 0; //Another Timer Delay For the go around using the proximity sensor
    unsigned int prox = 0; //Holds the Proximity value from the Proximity Sensor
    int didTurnAround = 0; //Questions if it turned 180 and go backwards. It should go once.
    initPWM(); //INilize PWM
    initADC(); //Inilize ADC
    initSW(); //Make the button on the development board works
    initI2C(); //Inilize I2c for the external sensor
    initVNCL4000(); //Inilize the sensor with the correct sensor to start working properly

    //Enable ON the H-bridge
    ENABLEPIN = OUTPUT;
    ENABLE = 1;

    while(1){
        switch(curState){
            case idle: //RObot starts at this state
                //Do nothing State
                LEFTWHEEL = 0; //Turns the wheel speed off which is 0, 1000 for max
                RIGHTWHEEL = 0;
                nextState = forward;
                break;
            case forward: //Sets the wheel to move forward
                //Change direct here
                PIN6 = 0; //pIn 5 and 7 is forward, Pin4 and 6 is backwards
                PIN4 = 0;
                PIN5 = 19; //Set these pins to be PWM pins
                PIN7 = 18;
                prevState = forward;
                curState = scan;
                //curState = keepRunning;
                break;
            case changeTires: //Reverse one of the wheel so that one of it is going forward and the other is going backwards
                PIN4 = 0;
                PIN5 = 19;
                PIN6 = 18;
                PIN7 = 0;
                curState = turnAround;
                break;
            case scan: //This turns on the adc to one sample from the infered sensor
                //All Detect
                AD1CON1bits.SAMP = 1; //Set the adc to start sampling
                if(AD1CON1bits.DONE == 1){ //Wait until it is done so it will switch states
                    curState = check;
                }
                break;
            case check:
                prox = readProximity();
                if (prox >= PROXIMITY_THRESHOLD){
                  curState = goAround;
                }
                //The 600 is the middle ground is between the value of white and black
                //This following statement means if the sensor sees all black it will initiate the turnaround state to go backwards
                else if ((adcVal1 <= 600 && adcVal2 <= 600 && adcVal3 <= 600 && adcVal4 <= 600)){ //All detecting
                    delay = 1;
                    if(didTurnAround == 0){
                        didTurnAround = 1; //We made it happen only once
                        curState = changeTires;
                    }
                    else{
                        curState = findLine;
                    }
                }
                //While the Two middle sensor is on the black line it will move forward
                else if(adcVal2 <= 600 && adcVal3 <= 600){
                    curState = moveForward;
                }
                //When one of the middle sensor start going off the black line it will correct itself
                else if(adcVal2 > 600 && adcVal3 <=600){
                    curState = moveRight;
                }
                //Same as the comment above
                else if(adcVal3 > 600 && adcVal2 <= 600){
                    curState = moveLeft;
                }
                //If all else fails it will fall into this state
                else {
                    curState = findLine;
                }
                break;
            case moveForward: //Sets the wheel to go forward
                RIGHTWHEEL = 600; 
                LEFTWHEEL = 600; //Make wheels move at more than half speed
                curState = forward;
                break;
            case moveRight:
                RIGHTWHEEL = 0; 
                LEFTWHEEL = 600;  
                curState = forward;
                break;
            case moveLeft:
                RIGHTWHEEL = 600;
                LEFTWHEEL = 0; 
                curState = forward;
                break;
            case turnAround:
                RIGHTWHEEL = 600; 
                LEFTWHEEL = 600;
                //Can add in function
                if(delay == 1){ //Don't let the code continue until it turn a certaina amount of time
                    delayS(1);
                    delay = 0;
                }
                AD1CON1bits.SAMP = 1; //Take samples until the middle two sensor sees black again
                if(AD1CON1bits.DONE == 1){
                    if(adcVal2 <= 600 && adcVal3 <= 600 && adcVal1 > 600 && adcVal4 > 600){
                    curState = forward;
                    }
                }   
                break;
            case goAround: //Proximity Routine
                if (goAroundCount == 0){
                    LEFTWHEEL = 400;
                    RIGHTWHEEL = 600;
                    delayMs(500);
                    goAroundCount = 1;
                }
                else{
                    LEFTWHEEL = 600;
                    RIGHTWHEEL = 400;
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
            case findLine: //This state use the most outer two infered sensor
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

    IFS0bits.AD1IF = 0; //Flag for addc
    adcVal1 = ADC1BUF0;
    adcVal2 = ADC1BUF1;
    adcVal3 = ADC1BUF2;
    adcVal4 = ADC1BUF3;
    AD1CON1bits.SAMP = 0; //Turn off sampling so it doesn't continue sampling forever
}
// ******************************************************************************************* //
void _ISR _CNInterrupt(void){
    IFS1bits.CNIF = 0;
    if(PORTBbits.RB5 == 1){
        curState = nextState;
    }
}

// ******************************************************************************************* //

