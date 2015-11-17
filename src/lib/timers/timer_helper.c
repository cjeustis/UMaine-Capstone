#include "timer_helper.h"
#include <avr/io.h>

void init_motors_timer(void) {
	// Normal operation of timer
	TCCR1A = 0x00;
	// Prescaler = 8
	TCCR1B = (1<<CS10);
	// Initialize counter to 0
	TCNT1 = 0;
	// No forced output compares
	TCCR1C = 0x00;
	// Initialize timer overflow count to 0
	tot_overflow = 0.0;
}

void enable_motors_timer_interrupt(void) {
	// Enable overflow interrupt
	TIMSK1 = (1<<TOIE1);
}

void disable_motors_timer_interrupt(void) {
	// Disable overflow interrupt
	TIMSK1 &= ~(1<<TOIE1);
}