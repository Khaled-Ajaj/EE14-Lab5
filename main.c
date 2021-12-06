#include "stm32l476xx.h"
#include "lcd.h"
#include <stdlib.h>

void delay(void);
void movingString(uint8_t* str, uint8_t len);

int main(void){
	LCD_Initialization();
	
	//LCD_Display_Name();
	
	while (1) {
		movingString((uint8_t *)"EE-14 is fun", 12);
	}
}

void delay(void) {
	// Delay loop
	int i;
	for (i = 0; i < 250000; i++);
}

void movingString(uint8_t* str, uint8_t len) {
	// Scrolls a string across the LCD
	uint8_t *cur = str; //pointer to the current start of the string
	uint8_t dot = 0;
	uint8_t colon = 0;
	
	int i;
	for(i = 0; i < len ;i++) { //loop over entire string
		int j;
		for(j = 0; j < 6; j++) { //loop through and print current 6 letters of the string
			//check to see if next char is a dot or colon
			if (*cur + i == ':') {
				colon = 1;
			}
			else {
				colon = 0;
			}
			if (*cur + i == '.') {
				dot = 1;
			}
			else {
				dot = 0;
			}
			LCD_WriteChar(cur+j,dot,colon,j); //print character currently being pointed to
		}
		cur = str + i; //set current character to the next character in the string
		delay(); //delay between LCD changes
	}
}
