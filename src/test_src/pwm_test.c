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
		OCR2A = voltage;		// Set OCR0A pin (PB3) voltage level

		_delay_ms(1);			// Delay to slow the rate of increase
	}
	PORTD |= _BV(PD7);
}

/* Disable a port using PWM */
void disable_port_pwm(void) {
    uint8_t voltage;

	for (voltage = 255; voltage > 0; voltage--) {
		OCR2A = voltage;		// Set OCR0A pin (PB3) voltage level

		_delay_ms(1);			// Delay so low the rate of decrease
	}
	PORTD &= ~_BV(PD7);
}

/* Initialize the PWM 8-bit timer/counter */
void init_pwm(void) {
	DDRD |= (1<<PD7);			// Use OCR0A pin (PB3)
	OCR2A = 0;					// Set initial value of pin
	TCCR2A |= (1<<COM2A1);		// Fast PWM, non-inverting & clear OC0B on compare-match, set OC0B at bottom
	TCCR2A |= (1<<WGM21) | (1<<WGM20);		// Fast PWM mode
	TCCR2B |= (1<<CS21);		// Prescaler = 8
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