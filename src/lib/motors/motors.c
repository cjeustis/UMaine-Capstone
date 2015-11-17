#include "motors.h"
#include <avr/io.h>
#include <stdbool.h>

/* Motor Information */
#define MOTOR1A_ON ~(1<<PC0)
#define MOTOR1B_ON (1<<PC1)
#define MOTOR2A_ON ~(1<<PC2)
#define MOTOR2B_ON (1<<PC3)
#define MOTOR3A_ON ~(1<<PC4)
#define MOTOR3B_ON (1<<PC5)
#define MOTOR4A_ON ~(1<<PC6)
#define MOTOR4B_ON (1<<PC7)

/* Define port to use for motors */
#define MOTORS_PORT PORTC
#define MOTORS_PORT_DIR DDRC

/* Pouring measurements - used for timing */
#define TENTH_OUNCE 490
#define QUARTER_OUNCE (2.5 * TENTH_OUNCE)
#define HALF_OUNCE (5.0 * TENTH_OUNCE)
#define OUNCE (10.0 * TENTH_OUNCE)

// 3.3814 ounces / minute
// 0.05635666666 ounces / second


// Pump = 1.67 mL / second
// 0.1 Oz = 2.95735 mL



// 1.774412965 seconds / 0.1 ounce
// 17.74412965 seconds / 1 ounce

static bool is_motor1_on = false;
static bool is_motor2_on = false;
static bool is_motor3_on = false;
static bool is_motor4_on = false;

void init_motors(void) {
	// Set ports as outputs for motors
	MOTORS_PORT_DIR = 0xFF;
	int i;
	for (i = 0; i < 4; i++) {
		motor_off(i);
	}
}

void motor_on(int motor) {
	switch(motor) {
		case 1:		// Turn on motor 1
			is_motor1_on = true;
			MOTORS_PORT &= MOTOR1A_ON;
			MOTORS_PORT |= MOTOR1B_ON;
			break;
		case 2:		// Turn on motor 2
			is_motor2_on = true;
			MOTORS_PORT &= MOTOR2A_ON;
			MOTORS_PORT |= MOTOR2B_ON;
			break;
		case 3:		// Turn on motor 3
			is_motor3_on = true;
			MOTORS_PORT &= MOTOR3A_ON;
			MOTORS_PORT |= MOTOR3B_ON;
			break;
		case 4:		// Turn on motor 4
			is_motor4_on = true;
			MOTORS_PORT &= MOTOR4A_ON;
			MOTORS_PORT |= MOTOR4B_ON;
			break;
	}
}

void motor_off(int motor) {
	switch(motor) {
		case 1:		// Turn on motor 1
			is_motor1_on = false;
			MOTORS_PORT &= MOTOR1A_ON;
			MOTORS_PORT &= ~MOTOR1B_ON;
			break;
		case 2:		// Turn on motor 2
			is_motor2_on = false;
			MOTORS_PORT &= MOTOR2A_ON;
			MOTORS_PORT &= ~MOTOR2B_ON;
			break;
		case 3:		// Turn on motor 3
			is_motor3_on = false;
			MOTORS_PORT &= MOTOR3A_ON;
			MOTORS_PORT &= ~MOTOR3B_ON;
			break;
		case 4:		// Turn on motor 4
			is_motor4_on = false;
			MOTORS_PORT &= MOTOR4A_ON;
			MOTORS_PORT &= ~MOTOR4B_ON;
			break;
	}
}