/* 
 * File:   pwm.h
 * Author: Stephen
 *
 * Created on March 23, 2015, 3:04 PM
 */

#ifndef PWM_H
#define	PWM_H

#define PIN4 RPOR0bits.RP0R
#define PIN5 RPOR0bits.RP1R
#define PIN6 RPOR4bits.RP8R
#define PIN7 RPOR4bits.RP9R

#define LEFTWHEEL OC2RS
#define RIGHTWHEEL OC1RS

#define ENABLEPIN TRISBbits.TRISB11

#define OUTPUT 0
#define ENABLE LATBbits.LATB11

void initPWM();

#endif	/* PWM_H */

