#define F_CPU 16000000UL

// Library files
#include "adc.c"
#include "usart.c"
#include "serial.c"

// Standard headers
#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>

// Set baud for serial communication
#define BAUD 9600
#define BAUDRATE ((F_CPU) / (BAUD * 16UL) - 1)

// Set usart info
#define TRANSMIT_RATE 1
#define DATA_BITS 8
#define STOP_BITS 1
#define PARITY_BITS 0

// Set adc info
#define REFERENCE 2
#define PRESCALER 128
#define AVERAGE 5


int main(int argc, const char *argv[])
{
    // Point stdout to serial stream (for testing to see adc value)
    stdout = &mystdout;

	// Initialize usart
	init_usart(BAUDRATE, TRANSMIT_RATE, DATA_BITS, STOP_BITS, PARITY_BITS);

	// Initialize ADC
	init_adc(REFERENCE, PRESCALER);

	while(1)
	{
		int i = 0;
		uint16_t adc = 0;

		for (i = 0; i < AVERAGE; i++)
		{
			adc += ReadADC(1);
		}

		adc /= AVERAGE;

		float voltage = adc * 5.0;
		voltage = voltage / 1024.0;

		printf("Voltage: %1.3f\n", voltage);

		// float tempC = (voltage - 0.5) * 100.0;
		// printf("Celcius: %1.2f\n", tempC);

		// float tempF = (tempC * 9.0 / 5.0) + 32.0;
		// printf("Temp (F): %1.2f*\n", tempF);

		_delay_ms(1000);
	}

	return 0;
}