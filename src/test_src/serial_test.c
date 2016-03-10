#define F_CPU 16000000UL

// Library files
#include <stdio.h>
#include <avr/io.h>
#include "../lib/usart/usart.c"
#include "../lib/serial/serial.c"

// Set baud for serial communication
#define BAUD 9600
#define BAUDRATE ((F_CPU) / (BAUD * 16UL) - 1)

// Set usart info
#define TRANSMIT_RATE 1
#define DATA_BITS 8
#define STOP_BITS 1
#define PARITY_BITS 0


int main(void) {

	// Initialize usart
	init_usart(BAUDRATE, TRANSMIT_RATE, DATA_BITS, STOP_BITS, PARITY_BITS);
	
	//assign our stream to standart I/O streams
	stdin=stdout=&usart0_str;

	printf("\e[1;1H\e[2J");

	char choice[1];
	printf("Mr. Pour!  The automated beverage creator!\n\n");
	printf("Created by: Chris Eustis\n            Slater Claudel\n\n");
	printf("             Menu:\n");
	printf("1. View available recipes\n");
	printf("2. Set internal temperature\n\n");

	while (1) {
		fgets(choice, 2, stdin);
		// scanf(" %1c", &choice);
		// getchar();

		if (choice[0] == '1') {
			printf("You entered '%c'\n", choice[0]);
		}
		else if (choice [0]== '2') {
			printf("You entered '%c'\n", choice[0]);
		}
		else {
			printf("You entered '%c', which is invalid\n", choice[0]);
		}
	}

	return 0;
}