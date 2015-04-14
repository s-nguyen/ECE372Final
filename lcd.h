/*
 * File:   lcd.h
 * Authors: Stephen Nguyen
 *
 * Created on December 31, 2014, 1:39 PM
 */
#ifndef LCD_H
#define LCD_H

#define LCD_DATA   LATB
#define LCD_RS  LATBbits.LATB7
#define LCD_E   LATBbits.LATB6
#define LCD_D7   LATBbits.LATB15
#define LCD_D6  LATBbits.LATB14
#define LCD_D5 LATBbits.LATB13
#define LCD_D4 LATBbits.LATB12

#define TRIS_D7  TRISBbits.TRISB15
#define TRIS_D6  TRISBbits.TRISB14
#define TRIS_D5  TRISBbits.TRISB13
#define TRIS_D4  TRISBbits.TRISB12
#define TRIS_RS  TRISBbits.TRISB7
#define TRIS_E   TRISBbits.TRISB6

#define LCD_WRITE_DATA 1
#define LCD_WRITE_CONTROL 0

#define LOWER 1
#define UPPER 0

void writeLCD(unsigned char word, unsigned int commandType, unsigned int delay);
void writeFourBits(unsigned char word, unsigned int commandType, unsigned int delayAfter, unsigned int lower);
void initLCD(void);
void clearLCD(void);
void moveCursorLCD(unsigned char x, unsigned char y);
void printCharLCD(char c);
void printStringLCD(const char* s);
void testLCD();

#endif