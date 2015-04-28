/*  //branch
 * File:   I2C.h
 * Author: Stephen
 *
 * Created on April 24, 2015, 1:01 PM
 */

#ifndef I2C_H
#define	I2C_H

void initI2C();
unsigned char readByte(char address);
void writeByte(char data, char address);
void beginTransmission(char saddress);
void send(char address);
void endTransmission();
void requestFrom(char saddress);
void masterACK();
void checkIdle();
#endif	/* I2C_H */

