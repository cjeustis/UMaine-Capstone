#include "motors.h"


/* Some useful tidbits about the timing of the pumps 
/   3.3814 ounces / minute
/   0.05635666666 ounces / second
/   Pump = 1.67 mL / second
/   0.1 Oz = 2.95735 mL
/   1.774412965 seconds / 0.1 ounce
/   17.74412965 seconds / 1 ounce */

/* Variable to check if a motor is currently operating */
static bool is_motor1_on = false;
static bool is_motor2_on = false;
static bool is_motor3_on = false;
static bool is_motor4_on = false;

/* Initialize the Ports used by the motors (defined above) and make sure all are turned off */
void init_motors(void) {
	MOTORS_PORT_DIR = 0xFF;					// Set ports as outputs for motors
	int i;
	for (i = 0; i < 4; i++) {
		motor_off(i);						// Turn all motors off
	}
}

/* Takes a motor designation as input and enables the motor */
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

/* Takes a motor designation as input and disables the motor */
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