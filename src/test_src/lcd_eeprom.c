#define F_CPU 16000000UL

// Standard headers
#include <avr/io.h>
#include <stdio.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <avr/eeprom.h>
#include <avr/interrupt.h>
#include "../Library/lcd/lcd.h"
#include "../Library/lcd/myutils.h"
#include "../Library/lcd/lcd.c"
#include "../Library/usart.c"
#include "../Library/serial.c"

// Set baud for serial communication
#define BAUD 9600
#define BAUDRATE ((F_CPU) / (BAUD * 16UL) - 1)

// Set usart info
#define TRANSMIT_RATE 1
#define DATA_BITS 8
#define STOP_BITS 1
#define PARITY_BITS 0

// Define recipe information
#define RECIPE_TOTAL_LENGTH 117
#define RECIPE_NAME_LENGTH 17
#define RECIPE_AMOUNT_LENGTH 4

// char EEMEM ee_total_recipe[RECIPE_TOTAL_LENGTH];
char EEMEM ee_recipe_name[RECIPE_NAME_LENGTH];

int main(int argc, const char *argv[])
{
	unsigned char i;
	LCDInit(LS_NONE);
	LCDClear();

	// Initialize usart
	init_usart(BAUDRATE, TRANSMIT_RATE, DATA_BITS, STOP_BITS, PARITY_BITS);

	char recipe_name[RECIPE_NAME_LENGTH];

    eeprom_read_block((void*)&recipe_name, (const void*)0x00, RECIPE_NAME_LENGTH);
    
	LCDWriteString("Recipe Name:");
	LCDWriteStringXY(0,1,recipe_name);

	for(i=0;i<25;i++) _delay_loop_2(0);
	
	// Clear second row
	LCDWriteStringXY(0,1,"                ");
    eeprom_read_block((void*)&recipe_name, (const void*)0x76, RECIPE_NAME_LENGTH);

	LCDWriteString("Recipe Name:");
	LCDWriteStringXY(0,1,recipe_name);

	return 0;
}