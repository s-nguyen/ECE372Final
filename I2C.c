#include "p24FJ64GA002.h"
#include "lcd.h"
#define WRITE 0
#define READ 1

#define VCNL4000_ADDRESS 0x13  // 0x26 write, 0x27 read



void initI2C(){
    TRISBbits.TRISB2 = 0;
    TRISBbits.TRISB3 = 0;
    AD1PCFGbits.PCFG4 = 1; //Configure those Pin to be digital
    AD1PCFGbits.PCFG5 = 1;

    
    I2C2CONbits.I2CSIDL = 0; //Stop in Idle Mode bit 0 = continue in idle mode
    I2C2CONbits.A10M = 0; //7 bit slave address
    I2C2CONbits.I2CEN = 1; //Enable Bit

    IFS3bits.MI2C2IF = 0;
    I2C2BRG = 144;
    
    
}

void writeByte(char reg, char data){
    I2C2CONbits.SEN = 1; //Start bit
    while(IFS3bits.MI2C2IF == 0);
    IFS3bits.MI2C2IF = 0; //wait

    I2C2TRN = VCNL4000_ADDRESS << 1 | WRITE;
    while(I2C2STATbits.TRSTAT == 1);
    if(I2C2STATbits.ACKSTAT == 0){
        I2C2TRN = reg;
        while(I2C2STATbits.TRSTAT == 1);
    }

    if(I2C2STATbits.ACKSTAT == 0){
        I2C2TRN = data;
        while(I2C2STATbits.TRSTAT == 1);
    }

    if(I2C2STATbits.ACKSTAT == 0){
        I2C2CONbits.PEN = 1;    //Stop
        while(IFS3bits.MI2C2IF == 0);     //wait
        IFS3bits.MI2C2IF = 0;
    }
    
    
}

char readByte(char reg){
    char c;

    I2C2CONbits.SEN = 1; //Start bit
    while(IFS3bits.MI2C2IF == 0);
    IFS3bits.MI2C2IF = 0;
    
    I2C2TRN = VCNL4000_ADDRESS << 1 | WRITE; //Send Slave address //Start transmitting when somethign is inserted
    while(I2C2STATbits.TRSTAT == 1); //Wait for address to be sent
    if(I2C2STATbits.ACKSTAT == 0){ // wait for ack
        I2C2TRN = reg;
        while(I2C2STATbits.TRSTAT == 1);
    }
    
    if(I2C2STATbits.ACKSTAT == 0){
        I2C2CONbits.PEN = 1;    //Stop
        while(IFS3bits.MI2C2IF == 0);     //wait
        IFS3bits.MI2C2IF = 0;
    }


    

    I2C2CONbits.SEN = 1; //Start bit
    while(IFS3bits.MI2C2IF == 0);
    IFS3bits.MI2C2IF = 0;

    I2C2TRN = VCNL4000_ADDRESS << 1 | READ; // Slave address
    while(I2C2STATbits.TRSTAT == 1); //Wait for address to be sent
    if(I2C2STATbits.ACKSTAT == 0){ // wait for ack
        while(I2C2STATbits.RBF == 0);
        c = I2C2RCV;
    }
    

    I2C2CONbits.ACKDT = 0;
    I2C2CONbits.ACKEN = 1;
    while(IFS3bits.MI2C2IF == 0);
    IFS3bits.MI2C2IF = 0;

    I2C2CONbits.PEN = 1;    //Stop
    while(IFS3bits.MI2C2IF == 0);     //wait
    IFS3bits.MI2C2IF = 0;

    return c;
}