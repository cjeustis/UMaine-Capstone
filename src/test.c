#define F_CPU 16000000UL

#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>
#include "lib/usart/usart.c"
#include "lib/serial/serial.c"

#define BAUD 		9600								// Set baud for serial communication
#define BAUDRATE 	((F_CPU) / (BAUD * 16UL) - 1)		// ----------------------------------

#define TRANSMIT_RATE 	1								// Set usart info
#define DATA_BITS 		8								// ---------------
#define STOP_BITS 		1	
#define PARITY_BITS 	0

int main(void)
{
	init_usart(BAUDRATE, TRANSMIT_RATE, DATA_BITS, STOP_BITS, PARITY_BITS);
	stdin = stdout = &usart0_str;

	char c;
	float ing1;
	float ing2;
	float ing3;
	float ing4;
	int pouring_amount;
	int temp;

	int isCooling = 1;

	while(1) {
		printf("Beginning forever loop");
		scanf(" %1c", &c);

		switch(c) {
			case 't':
				scanf(" %d", &temp);
				printf("Updated temp to: %d\n", temp);
				break;
			case 'p':
				// read the four ingredient amounts
				scanf(" %f", &ing1)
				scanf(" %f", &ing2)
				scanf(" %f", &ing3)
				scanf(" %f", &ing4)
				printf("Ingredient 1: %f\n", ing1);
				printf("Ingredient 2: %f\n", ing2);
				printf("Ingredient 3: %f\n", ing3);
				printf("Ingredient 4: %f\n", ing4);
				break;
			// default:
		}
	}
	return 0;
}