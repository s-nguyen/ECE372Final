/*
 * File:   timer.c
 * Authors: Stephen Nguyen
 *
 * Created on December 30, 2014, 8:07 PM
 */

#include "p24fj64ga002.h"
#include "timer.h"
#include <stdio.h>
#include <stdlib.h>

#define FCY 7372800.0*2
#define PRESCALE 8.0

//Uses timer 2

//14 is from FCY/1000000
void delayUs(unsigned int delay){
    TMR2 = 0;
    PR2 = 14*delay - 1;
    IFS0bits.T2IF = 0;
    T2CONbits.TCKPS = 0x0; //NO Prescale
    T2CONbits.TON = 1;
    while(IFS0bits.T2IF == 0){
        
    }
    T2CONbits.TON = 0;
}

void delayS(int s){
    TMR2 = 0;
    PR2 = (FCY/256) - 1;
    IFS0bits.T2IF = 0;
    T2CONbits.TCKPS = 0b11;
    T2CONbits.TON = 1;
    while(s != 0){
        while(IFS0bits.T2IF == 0){
        //Do nothing
        }
        IFS0bits.T2IF = 0;
        s = s - 1;
    }
    
    T2CONbits.TON = 0;
 
}
//Functions that always delays 5ms
void delay5ms(){
    TMR2 = 0;
    PR2 = (FCY*0.005)/PRESCALE - 1;
    IFS0bits.T2IF = 0;
    T2CONbits.TCKPS = 0b01;
    T2CONbits.TON = 1;
    while(IFS0bits.T2IF == 0){
        //Do nothing
    }
    T2CONbits.TON = 0;
}

void delayMs(int m){
    TMR2 = 0;
    PR2 = (FCY*.001)/PRESCALE - 1;
    IFS0bits.T2IF = 0;
    T2CONbits.TCKPS = 0b01;
    T2CONbits.TON = 1;
    while(m != 0){
        while(IFS0bits.T2IF == 0){
        //Do nothing
        }
        IFS0bits.T2IF = 0;
        m = m - 1;
    }

    T2CONbits.TON = 0;


}


//Turns a integer into miliseconds, seconds and minutes
//Str is the pass in Array that is preallocated to 9 spots to be filled
void getTimedString(int watch, char* str){
    int FF = watch % 100; //Get the lower 2 numbers
    int SS = (watch / 100) % 60; //MOD 60 to turn 60 seconds in a minute
    int MM = ((watch / 100) - SS) / 60; //Convert watch to minute
    
    
    sprintf(str, "%02d:%02d:%02d", MM, SS, FF); //Prints the number to a string

}

//Initilize timer1
void initTimer1(){
    TMR1 = 0;
    PR1 = (FCY*.01)/256 - 1;

    T1CONbits.TCKPS = 0b11;
    IFS0bits.T1IF = 0;
    IEC0bits.T1IE = 1;
}

void initSW(){
    //init a button?
    TRISBbits.TRISB5 = 1;
    IFS1bits.CNIF = 0;
    IEC1bits.CNIE = 1;
    CNEN2bits.CN27IE = 1;
}