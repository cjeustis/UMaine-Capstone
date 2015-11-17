#define F_CPU 16000000UL

// Library files
#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>
#include <avr/eeprom.h>
#include <avr/interrupt.h>
#include <util/delay.h>

// #include "lib/lcd/lcd.h"
// #include "lib/lcd/myutils.h"
// #include "lib/lcd/lcd.c"
#include "lib/main.h"
#include "lib/usart/usart.c"
#include "lib/serial/serial.c"
#include "lib/adc/adc.c"
#include "lib/recipes/recipes_object.c"
#include "lib/motors/motors.c"

// Set baud for serial communication
#define BAUD 9600
#define BAUDRATE ((F_CPU) / (BAUD * 16UL) - 1)

// Set adc info
#define REFERENCE 0
#define PRESCALER 128

// Set usart info
#define TRANSMIT_RATE 1
#define DATA_BITS 8
#define STOP_BITS 1
#define PARITY_BITS 0

#define USER_NAME_LENGTH 17
#define USER_AMOUNT_LENGTH 4

// Set temperature info
#define TEMP_LOWER 35
#define TEMP_UPPER 55
#define TEMP_OFFSET 240

// static uint8_t user_temperature;
volatile float tot_overflow;
// volatile float motor1_overflow;
// volatile float motor2_overflow;
// volatile float motor3_overflow;
// volatile float motor4_overflow;
static float pouring_length;

// Temp struct - needed? probably not
struct TempSensors {
	int channel;
	float temp0F;
	float temp1F;
	float tempFinal;
	int user_defined_temp;
} temps;

/* Timer/Counter 1 Overflow Interrupt (16-bit timer) */
ISR(TIMER1_OVF_vect) {
	// Keep track of overflows
	// if(is_motor1_on) {
	// 	motor1_overflow++;
	// }
	// if(is_motor2_on) {
	// 	motor2_overflow++;
	// }
	// if(is_motor3_on) {
	// 	motor3_overflow++;
	// }
	// if(is_motor4_on) {
	// 	motor4_overflow++;
	// }
	tot_overflow++;
}

/* ADC Conversion Interrupt */
ISR(ADC_vect) {
	if (temps.channel == 0) {
		temps.temp0F = convert_adc_to_fahrenheit(ADC+TEMP_OFFSET);
	}
	else if (temps.channel == 1) {
		temps.temp1F = convert_adc_to_fahrenheit(ADC+TEMP_OFFSET);

		temps.tempFinal = (temps.temp0F + temps.temp1F) / 2.0;
	}

	adc_disable_int();
}

void begin_pouring(int motor) {
	if(pouring_length != 0) {
		motor_on(motor);

		enable_motors_timer_interrupt();

		// Wait to be done pouring liquid
		while (tot_overflow < pouring_length);

		motor_off(motor);
		tot_overflow = 0;

		disable_motors_timer_interrupt();
	}
}

char* clean_string(int size, char string[]) {
	int i;
	for (i = 0; i < size; i++ )
	{
	    if ( string[i] == '\n' )
	    {
	        string[i] = '\0';
	        break;
	    }
	}
	return string;
}

void display_recipes(void) {
	int i;
	char choice;
	for (i = 0; i < NUMBER_OF_RECIPES; i++) {
		printf("--------------------\nRecipe %d\n--------------------\n", i+1);
		dumpRecipeState(recipes[i]);
	}
	
	while(1) {
		printf("\nPick a recipe (1-5) or type 'b' to go back: ");
		fgets(&choice, 1, stdin);

		switch(choice) {
			case '1':
				manage_recipe(0);
				break;
			case '2':
				manage_recipe(1);
				break;
			case '3':
				manage_recipe(2);
				break;
			case '4':
				manage_recipe(3);
				break;
			case '5':
				manage_recipe(4);
				break;
			case 'b':
				welcome_screen();	// return
		}
	}
}

void update_temperature(void) {
	char value;
	printf("\n\nThe temperature is currently set to: %d\n", temps.user_defined_temp);
	printf("Enter a value between 35 F and 55 F that you would like the internal temperature to be maintaned at: ");

	while(1) {
		fgets(&value, 2, stdin);
		value += '0';
		if ((value >= TEMP_LOWER) && (value <= TEMP_UPPER)) {
			temps.user_defined_temp = value;
			break;
		}
		printf("\n\nInvalid value. Enter a value between 35 F and 55 F that you would like the internal temperature to be maintaned at: ");
	}
	printf("\n\nThe desired internal temperature has been updated to %d F\n.", temps.user_defined_temp);
}

void set_temperature(void) {
	char choice;
	printf("\n\nMr. Pour: Temperature Control!\n--------------------\n\n");
	get_temp_reading();
	if (temps.tempFinal < 1) {
		get_temp_reading();
	}
	/* Display current Temperature */
	printf("Current temperature: %1.2f\n", temps.tempFinal);

	printf("\nWould you like to change the internal temperature? (y = yes, n = no): ");

	while(1) {
		fgets(&choice, 1, stdin);
		if (choice == 'y') {
			update_temperature();
			break;
		}
		else if (choice == 'n') {
			break;
		}
		printf("\n\nInvalid value. Would you like to change the internal temperature? (y = yes, n = no): ");
	}
	welcome_screen();
}

void welcome_screen(void) {
	char choice = '0';

	printf("\n\nMr. Pour!  The automated beverage creator!\n--------------------\n\n");
	printf("Created by: Chris Eustis\n            Slater Claudel\n\n");
	printf("Menu: \n");
	printf("1. View available recipes \n");
	printf("2. Set internal temperature \n");
	printf("Please select any option above by entering the item number: ");

	while (1) {
		fgets(&choice, 1, stdin);

		if (choice == '1') {
			display_recipes();
		}
		else if (choice == '2') {
			set_temperature();
		}
		printf("Invalid value. Please select any option above by entering the item number: ");
	}
}

int main(void) {
	// Initialize motors - ensure they are not on
	init_motors();
	// Set up timer for controlling liquids
	init_motors_timer();

	// Initialize usart
	init_usart(BAUDRATE, TRANSMIT_RATE, DATA_BITS, STOP_BITS, PARITY_BITS);

	// Initialize ADC
	init_adc(REFERENCE, PRESCALER);

	// Get recipes from eeprom
	init_recipes();

	// Set standard streams to serial
	stdin = stdout = &usart0_str;

	// Start off with internal temp set to 50 F
	temps.user_defined_temp = 50;

	// Enable global interrupts
	sei();	

	while(1) {
		// Start off with the welcome screen
		welcome_screen();

		// TODO: exit/standy-by/power-off?
	}
	return 1;
}