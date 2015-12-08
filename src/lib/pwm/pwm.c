#include "pwm.h"

/* Enable a port using PWM */
void enable_port_pwm(int pin) {
    uint8_t voltage;
	
	for (voltage = 0; voltage < 255; voltage++) {
		if (pin == 1) {
			OCR0A = voltage;		// Set OCR0A pin (PB3) voltage level		
		}

		_delay_ms(10);			// Delay to slow the rate of increase
	}
}

/* Disable a port using PWM */
void disable_port_pwm(int pin) {
    uint8_t voltage;

	for (voltage = 255; voltage > 0; voltage--) {
		if (pin == 1) {
			OCR0A = voltage;		// Set OCR0A pin (PB3) voltage level		
		}

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