#define F_CPU 16000000UL

// Library files
#include "../Library/adc.c"
#include "../Library/usart.c"
#include "../Library/serial.c"

// Standard headers
#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#include <avr/interrupt.h>

// Set baud for serial communication
#define BAUD 9600
#define BAUDRATE ((F_CPU) / (BAUD * 16UL) - 1)

// Set usart info
#define TRANSMIT_RATE 1
#define DATA_BITS 8
#define STOP_BITS 1
#define PARITY_BITS 0

// Set adc info
#define REFERENCE 0
#define PRESCALER 128
#define AVERAGE 10

// Set temperature info
#define TEMP_LOWER 35
#define TEMP_UPPER 55

struct TempSensors {
	int channel;
	float temp0F;
	float temp1F;
	float tempFinal;
} temps;


float convert_temp(uint16_t adc) {
	float c, v;
	/* Convert adc to voltage value */
	v = adc * 5.0;
	v = v / 1024.0;

	/* Convert voltage to celcius */
	c = (v - 0.5) * 10.0;

	/* Convert celcius to farenheit */
	return (c * 9.0 / 5.0) + 32.0;
}

ISR(ADC_vect) {
	if (temps.channel == 0) {
		temps.temp0F = convert_temp(ADC);
	}
	else if (temps.channel == 1) {
		temps.temp1F = convert_temp(ADC);

		temps.tempFinal = (temps.temp0F + temps.temp1F) / 2.0;
		printf("Temperature: %1.2f\n", temps.tempFinal);
	}

	adc_disable_int();
}

void temperature_value(void) {
	// Start ADC0 conversion
	adc_enable_int();
	temps.channel = 0;
	start_adc(0);
	_delay_ms(10);

	// Start ADC1 conversion
	adc_enable_int();
	temps.channel = 1;
	_delay_ms(10);
	start_adc(1);
}

int main(int argc, const char *argv[])
{
    // Point stdout to serial stream (for testing to see adc value)
    stdout = &mystdout;

	// Initialize usart
	init_usart(BAUDRATE, TRANSMIT_RATE, DATA_BITS, STOP_BITS, PARITY_BITS);

	// Initialize ADC
	init_adc(REFERENCE, PRESCALER);

	DDRB = 0x01;

	// Enable global interrupts
	sei();

	while(1)
	{
		temperature_value();

		if (temps.temp1F >= TEMP_UPPER) {
			PORTB = 0x01;
		}
		else {
			PORTB = 0x00;
		}
		// Wait a second
		_delay_ms(1000);
	}

	return 0;
}