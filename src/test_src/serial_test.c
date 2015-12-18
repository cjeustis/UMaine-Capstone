#define F_CPU 16000000UL

// Library files
#include <stdio.h>
#include <math.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include "../lib/usart/usart.c"
#include "../lib/serial/serial.c"
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

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
	char input[3];

	while(1) {
	    printf_P(PSTR("Press: a, b, 1, or 2: "));
	    if (!fgets(input, sizeof input, stdin)) {
	    	printf("ERROR!\n");
	    }
	    else if (isalnum((int)input) == 0) {
	    	printf("Invalid input\n");
	    }
	    else if (strcmp("a", input) == 0) {
	    	printf("You pressed 'a'\n");
	    }
	    else if (strcmp("b", input) == 0) {
	    	printf("You pressed 'b'\n");
	    }
	    else if (strcmp("1", input) == 0) {
	    	printf("You pressed '1'\n");
	    }
	    else if (strcmp("2", input) == 0) {
	    	printf("You pressed '2'\n");
	    }
	    else {
	    	printf("You entered invalid shit, fool\n");
	    }
	}

	return 0;
}