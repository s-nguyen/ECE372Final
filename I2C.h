/* 
 * File:   I2C.h
 * Author: Stephen
 *
 * Created on April 24, 2015, 1:01 PM
 */

#ifndef I2C_H
#define	I2C_H

void initI2C();
char readByte(char address);
void writeByte(char data, char address);

#endif	/* I2C_H */

