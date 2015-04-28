#include "p24fj64ga002.h"
#include "I2C.h"
#include "timer.h"


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

void initVNCL4000(){
    writeByte(AMBIENT_PARAMETER, 0x0F); //Single conversion mode, 128 average
    writeByte(IR_CURRENT, 20); // Set IR current to 200mA
    writeByte(PROXIMITY_FREQ, 3); // 390k Hz
    writeByte(PROXIMITY_MOD, 0x81); // 129
    delayUs(900);
}

unsigned int readProximity(){
    unsigned int data = 0;
    unsigned char temp;

    temp = readByte(COMMAND_0); 
    writeByte(COMMAND_0, 0b10001000/* temp | 0x08*/); // command the sensor to perform prxomity measure

    while(!(readByte(COMMAND_0)&0b00100000) /*0x20*/); // wait for prxomity data ready bit ot be set
    data = readByte(PROXIMITY_RESULT_MSB) << 8;
    data |= readByte(PROXIMITY_RESULT_LSB);

    return data;
}

unsigned int readAmbient(){
    unsigned int data;
    unsigned char temp;

    temp = readByte(COMMAND_0);
    writeByte(COMMAND_0, 0b10010000 /*temp | 0x10*/); // command the sensor to perform ambient measure

    while(!(readByte(COMMAND_0)&0b01000000) /*0x20*/); // wait for the proxmity data ready bit to be set
    data = readByte(AMBIENT_RESULT_MSB) << 8;
    data |= readByte(AMBIENT_RESULT_LSB);

    return data;
}

