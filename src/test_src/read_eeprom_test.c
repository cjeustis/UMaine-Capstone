#define F_CPU 16000000UL

// Library files
#include "../Library/usart.c"
#include "../Library/serial.c"

// Standard headers
#include <avr/io.h>
#include <stdio.h>
#include <avr/eeprom.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

// Set baud for serial communication
#define BAUD 9600
#define BAUDRATE ((F_CPU) / (BAUD * 16UL) - 1)

// Set usart info
#define TRANSMIT_RATE 1
#define DATA_BITS 8
#define STOP_BITS 1
#define PARITY_BITS 0

// Define recipe information
#define RECIPE_TOTAL_LENGTH 162
#define RECIPE_NAME_LENGTH 21
#define RECIPE_AMOUNT_LENGTH 2

// char EEMEM ee_total_recipe[RECIPE_TOTAL_LENGTH];
char EEMEM ee_recipe_name[RECIPE_NAME_LENGTH];

int main(int argc, const char *argv[])
{
    // Point stdout to serial stream (for testing to see adc value)
    stdout = &mystdout;

	// Initialize usart
	init_usart(BAUDRATE, TRANSMIT_RATE, DATA_BITS, STOP_BITS, PARITY_BITS);

	// printf("The first recipe is shown below!\n\n\n");

	char recipe_name[RECIPE_NAME_LENGTH];

    eeprom_read_block((void*)&recipe_name, (const void*)0x00, RECIPE_NAME_LENGTH);

    printf("Recipe Name: '%s'\n", recipe_name);

	return 0;
}