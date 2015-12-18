#include "pwm.h"

/* Enable a port using PWM */
void enable_port_pwm(int pin) {
    uint8_t voltage;
	
	for (voltage = 0; voltage < 255; voltage++) {
		if (pin == 1) {
			OCR2A = voltage;		// Set pin voltage level		
		}

		_delay_ms(10);			// Delay to slow the rate of increase
	}
}

/* Disable a port using PWM */
void disable_port_pwm(int pin) {
    uint8_t voltage;

	for (voltage = 255; voltage > 0; voltage--) {
		if (pin == 1) {
			OCR2A = voltage;		// Set pin voltage level		
		}

		_delay_ms(10);			// Delay so low the rate of decrease
	}
}

/* Initialize the PWM 8-bit timer/counter */
void init_pwm(void) {
	DDRD |= (1<<PD7);			// Use OCR0A pin (PB3)
	OCR2A = 0;					// Set initial value of pin
	TCCR2A |= (1<<COM2A1);		// Fast PWM, non-inverting & clear OC0B on compare-match, set OC0B at bottom
	TCCR2A |= (1<<WGM21) | (1<<WGM20);		// Fast PWM mode
	TCCR2B |= (1<<CS21);		// Prescaler = 8
}