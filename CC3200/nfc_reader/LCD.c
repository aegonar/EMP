#include <stdio.h>

#include "hw_types.h"
#include "hw_ints.h"
#include "hw_memmap.h"
#include "hw_common_reg.h"
#include "interrupt.h"
#include "hw_apps_rcm.h"
#include "prcm.h"
#include "pin.h"
#include "rom.h"
#include "rom_map.h"
#include "prcm.h"
#include "gpio.h"
#include "utils.h"

// Common interface includes
#include "pinmux.h"
#include "LCD.h"

void setEn(){
	MAP_GPIOPinWrite(E, E_ON);//E PIN_50
	MAP_UtilsDelay(enDelay);
	MAP_GPIOPinWrite(E, 0);//E PIN_50
}

void turnOnLcd(){
	/*Turn on lcd and cursor*/
	MAP_GPIOPinWrite(RS, 0);//RS PIN_60
	MAP_GPIOPinWrite(DB0, 0);//DB0 PIN_58
	MAP_GPIOPinWrite(DB1, DB1_ON);//DB1 PIN_03
	MAP_GPIOPinWrite(DB2, DB2_ON);//DB2 PIN_61
	MAP_GPIOPinWrite(DB3, DB3_ON);//DB3 PIN_59
	MAP_GPIOPinWrite(DB4, 0);//DB4 PIN_05
	MAP_GPIOPinWrite(DB5, 0);//DB5 PIN_62
	MAP_GPIOPinWrite(DB6, 0);//DB6 PIN_53
	MAP_GPIOPinWrite(DB7, 0);//DB7 PIN_63
}

void turnOffLcd()
{
	/*Turn off lcd and cursor*/
	MAP_GPIOPinWrite(RS, 0);//RS PIN_60
	MAP_GPIOPinWrite(DB0, 0);//DB0 PIN_58
	MAP_GPIOPinWrite(DB1, 0);//DB1 PIN_03
	MAP_GPIOPinWrite(DB2, 0);//DB2 PIN_61
	MAP_GPIOPinWrite(DB3, DB3_ON);//DB3 PIN_59
	MAP_GPIOPinWrite(DB4, 0);//DB4 PIN_05
	MAP_GPIOPinWrite(DB5, 0);//DB5 PIN_62
	MAP_GPIOPinWrite(DB6, 0);//DB6 PIN_53
	MAP_GPIOPinWrite(DB7, 0);//DB7 PIN_63
}


void setLineTwo()
{
	/*Set RAM*/
	MAP_GPIOPinWrite(RS, 0);//RS PIN_60
	MAP_GPIOPinWrite(DB0, 0);//DB0 PIN_58
	MAP_GPIOPinWrite(DB1, 0);//DB1 PIN_03
	MAP_GPIOPinWrite(DB2, 0);//DB2 PIN_61
	MAP_GPIOPinWrite(DB3, 0);//DB3 PIN_59
	MAP_GPIOPinWrite(DB4, 0);//DB4 PIN_05
	MAP_GPIOPinWrite(DB5, 0);//DB5 PIN_62
	MAP_GPIOPinWrite(DB6, DB6_ON);//DB6 PIN_53
	MAP_GPIOPinWrite(DB7, DB7_ON);//DB7 PIN_63
}

void goHome()
{
	/*Set RAM*/
	MAP_GPIOPinWrite(RS, 0);//RS PIN_60
	MAP_GPIOPinWrite(DB0, 0);//DB0 PIN_58
	MAP_GPIOPinWrite(DB1, DB1_ON);//DB1 PIN_03
	MAP_GPIOPinWrite(DB2, 0);//DB2 PIN_61
	MAP_GPIOPinWrite(DB3, 0);//DB3 PIN_59
	MAP_GPIOPinWrite(DB4, 0);//DB4 PIN_05
	MAP_GPIOPinWrite(DB5, 0);//DB5 PIN_62
	MAP_GPIOPinWrite(DB6, 0);//DB6 PIN_53
	MAP_GPIOPinWrite(DB7, 0);//DB7 PIN_63
}

void setLineOne()
{
	/*Set Line 1 Position 0*/
	MAP_GPIOPinWrite(RS, 0);//RS PIN_60
	MAP_GPIOPinWrite(DB0, 0);//DB0 PIN_58
	MAP_GPIOPinWrite(DB1, 0);//DB1 PIN_03
	MAP_GPIOPinWrite(DB2, 0);//DB2 PIN_61
	MAP_GPIOPinWrite(DB3, 0);//DB3 PIN_59
	MAP_GPIOPinWrite(DB4, 0);//DB4 PIN_05
	MAP_GPIOPinWrite(DB5, 0);//DB5 PIN_62
	MAP_GPIOPinWrite(DB6, 0);//DB6 PIN_53
	MAP_GPIOPinWrite(DB7, DB7_ON);//DB7 PIN_63

}

void functionSet(){
	/*Function Set to 2 lines, 8 bit bus, and 5x8 dot format*/
	MAP_GPIOPinWrite(RS, 0);//RS PIN_60
	MAP_GPIOPinWrite(DB0, 0);//DB0 PIN_58
	MAP_GPIOPinWrite(DB1, 0);//DB1 PIN_03
	MAP_GPIOPinWrite(DB2, 0);//DB2 PIN_61
	MAP_GPIOPinWrite(DB3, DB3_ON);//DB3 PIN_59
	MAP_GPIOPinWrite(DB4, DB4_ON);//DB4 PIN_05
	MAP_GPIOPinWrite(DB5, DB5_ON);//DB5 PIN_62
	MAP_GPIOPinWrite(DB6, 0);//DB6 PIN_53
	MAP_GPIOPinWrite(DB7, 0);//DB7 PIN_63
	}

void entryModeSet(){
	/*Entry Mode set*/
	MAP_GPIOPinWrite(RS, 0);//RS PIN_60
	MAP_GPIOPinWrite(DB0, 0);//DB0 PIN_58
	MAP_GPIOPinWrite(DB1, DB1_ON);//DB1 PIN_03
	MAP_GPIOPinWrite(DB2, 0);//DB2 PIN_61
	MAP_GPIOPinWrite(DB3, 0);//DB3 PIN_59
	MAP_GPIOPinWrite(DB4, 0);//DB4 PIN_05
	MAP_GPIOPinWrite(DB5, 0);//DB5 PIN_62
	MAP_GPIOPinWrite(DB6, 0);//DB6 PIN_53
	MAP_GPIOPinWrite(DB7, 0);//DB7 PIN_63
}

void clearDisplay(){
	/*clear data form lcd*/
	MAP_GPIOPinWrite(RS, 0);//RS PIN_60
	MAP_GPIOPinWrite(DB0, DB0_ON);//DB0 PIN_58
	MAP_GPIOPinWrite(DB1, 0);//DB1 PIN_03
	MAP_GPIOPinWrite(DB2, 0);//DB2 PIN_61
	MAP_GPIOPinWrite(DB3, 0);//DB3 PIN_59
	MAP_GPIOPinWrite(DB4, 0);//DB4 PIN_05
	MAP_GPIOPinWrite(DB5, 0);//DB5 PIN_62
	MAP_GPIOPinWrite(DB6, 0);//DB6 PIN_53
	MAP_GPIOPinWrite(DB7, 0);//DB7 PIN_63
}

void setCharInitPos(){
	/*Set Line 1 Position 0*/
	MAP_GPIOPinWrite(RS, 0);//RS PIN_60
	MAP_GPIOPinWrite(DB0, 0);//DB0 PIN_58
	MAP_GPIOPinWrite(DB1, 0);//DB1 PIN_03
	MAP_GPIOPinWrite(DB2, 0);//DB2 PIN_61
	MAP_GPIOPinWrite(DB3, 0);//DB3 PIN_59
	MAP_GPIOPinWrite(DB4, 0);//DB4 PIN_05
	MAP_GPIOPinWrite(DB5, 0);//DB5 PIN_62
	MAP_GPIOPinWrite(DB6, 0);//DB6 PIN_53
	MAP_GPIOPinWrite(DB7, DB7_ON);//DB7 PIN_63

}

void writeLCD(char line1[]){

	goHome();
	setEn();
	int i=0;
	int j=0;
	while(i<16){
		char temp = line1[i];
		int current[7];
		int num =0;
		j=0;
		while(j<7){
			num = temp%2;
			temp = temp/2;

			if(num == 1){
				current[j] = 1;
			}
			else{
				current[j]=0;
			}
			j++;
		}

		MAP_UtilsDelay(2000);
		MAP_GPIOPinWrite(RS, RS_ON);//RS PIN_60

		if(current[0] == 0){
			MAP_GPIOPinWrite(DB0, 0);//DB0 PIN_58
		}
		else{
			MAP_GPIOPinWrite(DB0, DB0_ON);//DB0 PIN_58
		}

		if(current[1] == 0){
			MAP_GPIOPinWrite(DB1, 0);//DB0 PIN_58
		}
		else{
			MAP_GPIOPinWrite(DB1, DB1_ON);//DB0 PIN_58
		}

		if(current[2] == 0){
			MAP_GPIOPinWrite(DB2, 0);//DB0 PIN_58
		}
		else{
			MAP_GPIOPinWrite(DB2, DB2_ON);//DB0 PIN_58
		}

		if(current[3] == 0){
			MAP_GPIOPinWrite(DB3, 0);//DB0 PIN_58
		}
		else{
			MAP_GPIOPinWrite(DB3, DB3_ON);//DB0 PIN_58
		}

		if(current[4] == 0){
			MAP_GPIOPinWrite(DB4, 0);//DB0 PIN_58
		}
		else{
			MAP_GPIOPinWrite(DB4, DB4_ON);//DB0 PIN_58
		}

		if(current[5] == 0){
			MAP_GPIOPinWrite(DB5, 0);//DB0 PIN_58
		}
		else{
			MAP_GPIOPinWrite(DB5, DB5_ON);//DB0 PIN_58
		}

		if(current[6] == 0){
			MAP_GPIOPinWrite(DB6, 0);//DB0 PIN_58
		}
		else{
			MAP_GPIOPinWrite(DB6, DB6_ON);//DB0 PIN_58
		}

		MAP_GPIOPinWrite(DB7, 0);//DB7 PIN_63
		MAP_UtilsDelay(charDelay);
		setEn();
		i=i+1;
	}
}

void writeLines(char line1[], char line2[]){

	goHome();
	setEn();
	int i=0;
	int j=0;
	while(i<16){
		char temp = line1[i];
		int current[7];
		int num =0;
		j=0;
		while(j<7){
			num = temp%2;
			temp = temp/2;

			if(num == 1){
				current[j] = 1;
			}
			else{
				current[j]=0;
			}
			j++;
		}

		MAP_UtilsDelay(2000);
		MAP_GPIOPinWrite(RS, RS_ON);//RS PIN_60

		if(current[0] == 0){
			MAP_GPIOPinWrite(DB0, 0);//DB0 PIN_58
		}
		else{
			MAP_GPIOPinWrite(DB0, DB0_ON);//DB0 PIN_58
		}

		if(current[1] == 0){
			MAP_GPIOPinWrite(DB1, 0);//DB0 PIN_58
		}
		else{
			MAP_GPIOPinWrite(DB1, DB1_ON);//DB0 PIN_58
		}

		if(current[2] == 0){
			MAP_GPIOPinWrite(DB2, 0);//DB0 PIN_58
		}
		else{
			MAP_GPIOPinWrite(DB2, DB2_ON);//DB0 PIN_58
		}

		if(current[3] == 0){
			MAP_GPIOPinWrite(DB3, 0);//DB0 PIN_58
		}
		else{
			MAP_GPIOPinWrite(DB3, DB3_ON);//DB0 PIN_58
		}

		if(current[4] == 0){
			MAP_GPIOPinWrite(DB4, 0);//DB0 PIN_58
		}
		else{
			MAP_GPIOPinWrite(DB4, DB4_ON);//DB0 PIN_58
		}

		if(current[5] == 0){
			MAP_GPIOPinWrite(DB5, 0);//DB0 PIN_58
		}
		else{
			MAP_GPIOPinWrite(DB5, DB5_ON);//DB0 PIN_58
		}

		if(current[6] == 0){
			MAP_GPIOPinWrite(DB6, 0);//DB0 PIN_58
		}
		else{
			MAP_GPIOPinWrite(DB6, DB6_ON);//DB0 PIN_58
		}

		MAP_GPIOPinWrite(DB7, 0);//DB7 PIN_63
		MAP_UtilsDelay(charDelay);
		setEn();
		i=i+1;
	}

	setLineTwo();
	setEn();
	i=0;
	j=0;
	while(i<16){
		char temp = line2[i];
		int current[7];
		int num =0;
		j=0;
		while(j<7){
			num = temp%2;
			temp = temp/2;

			if(num == 1){
				current[j] = 1;
			}
			else{
				current[j]=0;
			}
			j++;
		}

		MAP_UtilsDelay(2000);
		MAP_GPIOPinWrite(RS, RS_ON);//RS PIN_60

		if(current[0] == 0){
			MAP_GPIOPinWrite(DB0, 0);//DB0 PIN_58
		}
		else{
			MAP_GPIOPinWrite(DB0, DB0_ON);//DB0 PIN_58
		}

		if(current[1] == 0){
			MAP_GPIOPinWrite(DB1, 0);//DB0 PIN_58
		}
		else{
			MAP_GPIOPinWrite(DB1, DB1_ON);//DB0 PIN_58
		}

		if(current[2] == 0){
			MAP_GPIOPinWrite(DB2, 0);//DB0 PIN_58
		}
		else{
			MAP_GPIOPinWrite(DB2, DB2_ON);//DB0 PIN_58
		}

		if(current[3] == 0){
			MAP_GPIOPinWrite(DB3, 0);//DB0 PIN_58
		}
		else{
			MAP_GPIOPinWrite(DB3, DB3_ON);//DB0 PIN_58
		}

		if(current[4] == 0){
			MAP_GPIOPinWrite(DB4, 0);//DB0 PIN_58
		}
		else{
			MAP_GPIOPinWrite(DB4, DB4_ON);//DB0 PIN_58
		}

		if(current[5] == 0){
			MAP_GPIOPinWrite(DB5, 0);//DB0 PIN_58
		}
		else{
			MAP_GPIOPinWrite(DB5, DB5_ON);//DB0 PIN_58
		}

		if(current[6] == 0){
			MAP_GPIOPinWrite(DB6, 0);//DB0 PIN_58
		}
		else{
			MAP_GPIOPinWrite(DB6, DB6_ON);//DB0 PIN_58
		}

		MAP_GPIOPinWrite(DB7, 0);//DB7 PIN_63
		MAP_UtilsDelay(charDelay);
		setEn();
		i=i+1;
	}
}

void writef(char line[]){
		char tmp[16];
		sprintf(tmp, "%-16s", line);		//format string
		write(tmp);
}

void initLCD(){
	//Wait time greater than 15ms after VDD=4.5V on LCD
	 MAP_UtilsDelay(400000);
	 functionSet();
	 setEn();
	 turnOnLcd();
	 setEn();
	 turnOffLcd();
	 setEn();
	 clearDisplay();
	 setEn();
	 entryModeSet();
	 setEn();
	 turnOnLcd();
	 setEn();
}







