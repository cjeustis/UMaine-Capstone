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
	char motor;
	int pouring_amount;

	int isCooling = 1;

	while(1) {
		printf("Beginning forever loop");
		scanf(" %1c", &c);

		switch(c) {
			case '0':
				printf("Received a '0'\n");
				if (isCooling) {
					printf("Turned cooler off\n");
					isCooling = 0;
				} else {
					printf("Cooler is already off\n");
				}
				break;
			case '1':
				printf("Received a '1'\n");
				if (!isCooling)
					printf("Turned cooler on\n");
					isCooling = 1;
				} else {
					printf("Cooler is already on\n");
				}
				break;
			case 'p':
				printf("Received a 'p' - Begin process of pouring a recipe\n");
				while (1) {
					scanf(" %1c", &motor);
					scanf(" %d", &pouring_amount);
					printf("Pouring motor %c for %d ounces\n", motor, pouring_amount);
					break;
				}
				break;
			// default:
		}
	}
	return 0;
}