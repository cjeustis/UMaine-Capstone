#define F_CPU 16000000UL

#include <avr/io.h>
#include <stdio.h>
#include <stdbool.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdlib.h>
#include "lib/usart/usart.c"
#include "lib/serial/serial.c"
#include "lib/motors/motors.c"
#include "lib/timers/timer_helper.c"

#define BAUD 		9600
#define BAUDRATE 	((F_CPU) / (BAUD * 16UL) - 1)

#define TRANSMIT_RATE 	1
#define DATA_BITS 		8
#define STOP_BITS 		1	
#define PARITY_BITS 		0

#define TENTH_OUNCE 	277.5
#define QUARTER_OUNCE 	(2.5 * TENTH_OUNCE)
#define HALF_OUNCE 		(5.0 * TENTH_OUNCE)
#define OUNCE 		(10.0 * TENTH_OUNCE)

typedef struct {
	bool isCooling;
	float defined_temp;
	float sensor_temp;
} TempData;

typedef struct {
	float ing1;
	float ing2;
	float ing3;
	float ing4;
} Recipe;

/* Timer/Counter 1 Overflow Interrupt (16-bit timer) */
ISR(TIMER1_OVF_vect)
{
	motors_overflow_count++;
}

void compare_temps(TempData *temps)
{
	printf("Defined temp: %d\n", (int)temps->defined_temp);
	printf("Sensor temp: %f\n", (double)temps->sensor_temp);

	if (temps->sensor_temp > temps->defined_temp) {
		if (!temps->isCooling) {
			printf("Cooler has been turned on!\n");
			temps->isCooling = true;
			PORTD |= _BV(PD7);
		}
	} else {
		if (temps->sensor_temp <= temps->defined_temp-1.5) {
			printf("Cooler has been turned off!\n");
			if (temps->isCooling) {
				temps->isCooling = false;
				PORTD &= ~_BV(PD7);
			}
		}
	}
}
/* Pour a given recipe */
void pour_recipe(Recipe *recipe)
{
	pouring_length = (recipe->ing1 * OUNCE);
	if (pouring_length != 0) {
		printf("Pouring ingredient one: %f\n", (double)recipe->ing1);
		pouring_length += MOTOR1_OFFSET;
		enable_motor_timer(1);
		_delay_ms(100);
	}

	pouring_length = (recipe->ing2 * OUNCE);
	if (pouring_length != 0) {
		printf("Pouring ingredient two: %f\n", (double)recipe->ing2);
		pouring_length += MOTOR2_OFFSET;
		enable_motor_timer(2);
		_delay_ms(100);
	}

	pouring_length = (recipe->ing3 * OUNCE);
	if (pouring_length != 0) {
		printf("Pouring ingredient three: %f\n", (double)recipe->ing3);
		pouring_length += MOTOR3_OFFSET;
		enable_motor_timer(3);
		_delay_ms(100);
	}

	pouring_length = (recipe->ing4 * OUNCE);
	if (pouring_length != 0) {
		printf("Pouring ingredient four: %f\n", (double)recipe->ing4);
		pouring_length += MOTOR4_OFFSET;
		enable_motor_timer(4);
	}
}

/* Converts the recipe's ingredient's amounts from char to float to use when dispensing liquids */
void convert_temp_to_float(TempData *temps, char *temp, int c) {
	if (c == 0)
		temps->sensor_temp = atof(temp);
	else
		temps->defined_temp = atof(temp);
}

/* Converts the recipe's ingredient's amounts from char to float to use when dispensing liquids */
void convert_recipe_to_float(Recipe *recipe, char *ing1, char *ing2, char *ing3, char *ing4) {
	recipe->ing1 = atof(ing1);
	recipe->ing2 = atof(ing2);
	recipe->ing3 = atof(ing3);
	recipe->ing4 = atof(ing4);
}

void init_peltier_port(void)
{
	DDRD = _BV(PD7);
	PORTD |= _BV(PD7);
}

void perform_inits(void)
{
	init_usart(BAUDRATE, TRANSMIT_RATE, DATA_BITS, STOP_BITS, PARITY_BITS);
	init_peltier_port();
	init_motors();
	init_motors_timer();
	sei();
}


int main(void)
{
	perform_inits();

	stdin = stdout = &usart0_str;

	TempData *temps = malloc(sizeof(TempData));
	temps->isCooling = true;
	temps->defined_temp = 35.;
	temps->sensor_temp = 0.;

	Recipe *recipe = malloc(sizeof(Recipe));
	recipe->ing1 = 0.;
	recipe->ing2 = 0.;
	recipe->ing3 = 0.;
	recipe->ing4 = 0.;

	char c;

	char *ing1 = malloc(3 * sizeof(char));
	char *ing2 = malloc(3 * sizeof(char));
	char *ing3 = malloc(3 * sizeof(char));
	char *ing4 = malloc(3 * sizeof(char));

	char *sensor_temp = malloc(4 * sizeof(char));
	char *defined_temp = malloc(2 * sizeof(char));

	while(1) {
		printf("Beginning of loop!\n");
		scanf(" %c", &c);

		switch(c) {
			case 'c':
			// get value from temp sensor
			// compare with set temp
			// decide whether cooler is on/off
				printf("Get temp sensor data\n");
				scanf(" %s", sensor_temp);
				convert_temp_to_float(temps, sensor_temp, 0);
				if (temps->sensor_temp != 0.)
					compare_temps(temps);
				else
					printf("Invalid temp - not doing anything\n");
				break;
			case 'p':
			// read the four ingredient amounts
			// turn each motor on to pour the ingredients
				printf("Getting ingredient amounts\n");
				scanf(" %s", ing1);
				scanf(" %s", ing2);
				scanf(" %s", ing3);
				scanf(" %s", ing4);
				convert_recipe_to_float(recipe, ing1, ing2, ing3, ing4);
				pour_recipe(recipe);
				break;
			case 't':
			// grab updated internal temp from site
				scanf(" %s", defined_temp);
				convert_temp_to_float(temps, defined_temp, 1);
				printf("Updated temp to: %d\n", (int)temps->defined_temp);
				break;
		}
	}
	free(ing1);
	free(ing2);
	free(ing3);
	free(ing4);
	free(sensor_temp);
	free(defined_temp);
	free(temps);
	free(recipe);
	return 0;
}