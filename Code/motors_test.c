#define F_CPU 16000000UL

#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <string.h>
#include "../Library/motors.c"

#define second 122
#define length 10
#define pourLength second * length

char forward[] = "forward";
char reverse[] = "reverse";

volatile float tot_overflow;
static float pouring_length;

// Reference for L293D H-Bridge
// http://www.robotplatform.com/howto/L293/motor_driver_1.html

// bool StringsAreEqual(char str1[], char str2[])
// {
// 	if (strcmp(str1, str2) == 0)
// 	{
// 		return true;
// 	}
// 	else
// 	{
// 		return false;
// 	}
// }


/* Timer/Counter 1 Overflow Interrupt (16-bit timer)
* Not used now because checking/clearing flag ourselves in main function */
ISR(TIMER1_OVF_vect) {
	// Keep track of overflows
	tot_overflow++;

	// if (tot_overflow >= pouring_length) {
	// 	motor_off(1);
	// 	tot_overflow = 0;
	// }
}

void init_timer(void) {
	// Normal operation of timer
	TCCR1A = 0x00;
	// Prescaler = 8
	TCCR1B = (1<<CS11);
	// Initialize counter to 0
	TCNT1 = 0;
	// No forced output compares
	TCCR1C = 0x00;
	// Enable overflow interrupt
	TIMSK1 = (1<<TOIE1);
	// Initialize timer overflow count to 0
	tot_overflow = 0.0;
}

int main(int argc, const char *argv[])
{
	init_motors();
	init_timer();

	// Set values of ingredients to pour
	// Link ing1 to motor 1, ing2 to motor2, etc.
	float ing1 = 0.1;	// Let be motor 1
	float ing2 = 0.2;	// Let be motor 2
	float ing3 = 0.1;	// Let be motor 3
	float ing4 = 0.4;	// Let be motor 4

	// Pour first ingredient
	pouring_length = (ing1 * OUNCE);
	motor_on(1);
	sei();

	// Wait to be done pouring liquid
	while (tot_overflow < pouring_length);

	motor_off(1);
	tot_overflow = 0;
	cli();

	// // Pour first ingredient
	// pouring_length = (ing1 * OUNCE);
	// motor_on(2);
	// sei();

	// // Wait to be done pouring liquid
	// while (tot_overflow < pouring_length);

	// motor_off(2);
	// tot_overflow = 0;
	// cli();

	// // Pour first ingredient
	// pouring_length = (ing1 * OUNCE);
	// motor_on(3);
	// sei();

	// // Wait to be done pouring liquid
	// while (tot_overflow < pouring_length);

	// motor_off(3);
	// tot_overflow = 0;
	// cli();

	// // Pour first ingredient
	// pouring_length = (ing1 * OUNCE);
	// motor_on(4);
	// sei();

	// // Wait to be done pouring liquid
	// while (tot_overflow < pouring_length);

	// motor_off(4);
	// tot_overflow = 0;
	// cli();

	return 0;
}