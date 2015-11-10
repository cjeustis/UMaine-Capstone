#define F_CPU 16000000UL

// Library files
#include <stdio.h>
#include <math.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
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


int main(void) {

	// Initialize usart
	init_usart(BAUDRATE, TRANSMIT_RATE, DATA_BITS, STOP_BITS, PARITY_BITS);
	
	//assign our stream to standart I/O streams
	stdin=stdout=&usart0_str;
	uint8_t u8Data;

	while(1) {
		printf("THIS IS A TEST\n\n");
	    printf_P(PSTR("\nPress any key:"));
	    //scan standard stream (USART)
	    scanf("%c",&u8Data);
	    printf_P(PSTR("\nYou pressed: "));
	    //print scaned character and its code
	    printf("%c; Key code: %u",u8Data, u8Data);
	}

	return 0;
}