/*
 * LCD.h
 *
 *  Created on: Oct 27, 2015
 *      Author: Alejandro Gonzalez
 */

#ifndef LCD_H_
#define LCD_H_





#endif /* LCD_H_ */


#define E GPIOA0_BASE, 0x1
#define E_ON 0x1

#define RS GPIOA0_BASE, 0x20
#define RS_ON 0x20

#define DB0 GPIOA2_BASE, 0x2
#define DB0_ON 0x2

#define DB1 GPIOA3_BASE, 0x80
#define DB1_ON 0x80

#define DB2 GPIOA0_BASE, 0x40
#define DB2_ON 0x40

#define DB3 GPIOA3_BASE, 0x10
#define DB3_ON 0x10

#define DB4 GPIOA1_BASE, 0x40
#define DB4_ON 0x40

#define DB5 GPIOA0_BASE, 0x80
#define DB5_ON 0x80

#define DB6 GPIOA3_BASE, 0x40
#define DB6_ON 0x40

#define DB7 GPIOA1_BASE, 0x1
#define DB7_ON 0x1

static const unsigned long int enDelay=(100000);
static const unsigned long int startUpDelay=(450e3);
static const unsigned long int sStartUpDelay=(120e3);
static const unsigned long int tStartUpDelay=(4e3);
static const unsigned long int charDelay=(2000);

extern void setEn(void);

extern void turnOnLcd(void);

extern void turnOffLcd(void);

extern void setLineTwo(void);

extern void goHome(void);

extern void setLineOne(void);

extern void functionSet(void);

extern void entryModeSet(void);

extern void clearDisplay();

extern void setCharInitPos(void);

extern void writeLCD(char line1[]);

extern void writeLines(char line1[], char line2[]);

extern void writef(char line[]);

extern void initLCD();







