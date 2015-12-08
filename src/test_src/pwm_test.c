#define F_CPU 16000000UL

// Standard headers
#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include <util/delay.h>

/* Enable a port using PWM */
void enable_port_pwm(void) {
    uint8_t voltage;
	
	for (voltage = 0; voltage < 255; voltage++) {
		OCR0A = voltage;		// Set OCR0A pin (PB3) voltage level

		_delay_ms(10);			// Delay to slow the rate of increase
	}
}

/* Disable a port using PWM */
void disable_port_pwm(void) {
    uint8_t voltage;

	for (voltage = 255; voltage > 0; voltage--) {
		OCR0A = voltage;		// Set OCR0A pin (PB3) voltage level

		_delay_ms(10);			// Delay so low the rate of decrease
	}
}

/* Initialize the PWM 8-bit timer/counter */
void init_pwm(void) {
	DDRB |= (1<<PB3);			// Use OCR0A pin (PB3)
	OCR0A = 0;					// Set initial value of pin
	TCCR0A |= (1<<COM0A1);		// Fast PWM, non-inverting & clear OC0B on compare-match, set OC0B at bottom
	TCCR0A |= (1<<WGM01) | (1<<WGM00);		// Fast PWM mode
	TCCR0B |= (1<<CS01);		// Prescaler = 8
}

int main(int argc, const char *argv[])
{
    init_pwm();

	while(1) {
		enable_port_pwm();
		_delay_ms(1000);
		disable_port_pwm();
		_delay_ms(1000);
	}

	return 0;
}