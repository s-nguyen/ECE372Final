/* 
 * File:   VCNL4000.h
 * Author: Stephen
 *
 * Created on April 24, 2015, 6:59 PM
 */

#ifndef VCNL4000_H
#define	VCNL4000_H

void initVNCL4000();
#endif


#include "p24fj64ga002.h"
#include "I2C.h"

#define VCNL4000_ADDRESS 0x13  // 0x26 write, 0x27 read

// VCNL4000 Register Map
#define COMMAND_0 0x80  // starts measurments, relays data ready info
#define PRODUCT_ID 0x81  // product ID/revision ID, should read 0x11
#define IR_CURRENT 0x83  // sets IR current in steps of 10mA 0-200mA
#define AMBIENT_PARAMETER 0x84  // Configures ambient light measures
#define AMBIENT_RESULT_MSB 0x85  // high byte of ambient light measure
#define AMBIENT_RESULT_LSB 0x86  // low byte of ambient light measure
#define PROXIMITY_RESULT_MSB 0x87  // High byte of proximity measure
#define PROXIMITY_RESULT_LSB 0x88  // low byte of proximity measure
#define PROXIMITY_FREQ 0x89  // Proximity IR test signal freq, 0-3
#define PROXIMITY_MOD 0x8A  // proximity modulator timing

void initVNCL4000();
unsigned int readProximity();
unsigned int readAmbient();


#endif	/* VCNL4000_H */

