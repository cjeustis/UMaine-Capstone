#define F_CPU 16000000UL

/* Standard headers */
#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>
#include <avr/eeprom.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdbool.h>
#include <string.h>

/* Helper files */
#include "lib/main.h"
#include "lib/usart/usart.c"
#include "lib/serial/serial.c"
#include "lib/adc/adc.c"
#include "lib/recipes/recipes_object.c"
#include "lib/motors/motors.c"
#include "lib/timers/timer_helper.c"
#include "lib/temp/temp_helper.c"

/* INTERRUPT PRIORITY:
/  16. Timer/Counter1 Overflow
/  25. ADC Convertsion Complete
/  35. Timer/Counter3 Overflow
*/

/* Timer/Counter 1 Overflow Interrupt (16-bit timer) */
ISR(TIMER1_OVF_vect) {
	motors_overflow_count++;			// Increase overflow count for the motor that is on
}

/* Timer/Counter 3 Overflow Interrupt (16-bit timer) */
ISR(TIMER3_OVF_vect) {
	temp_overflow_count++;			// Increase overflow count to track when to read temp

	/* Check the temp every ~10 seconds */
	if (temp_overflow_count >= TEN_SECONDS) {
		get_internal_temp();			// Get the temp value
		if (temps.tempFinal > temps.user_defined_temp) {
			PORTB = 0x01;				// Turn on Peltier module and start cooling it down
		}
		else {
			PORTB = 0x00;				// If not to hot, make sure Peltier is off
		}
		temp_overflow_count = 0;		// Reset overflow count
	}
}

/* ADC Conversion Interrupt */
ISR(ADC_vect) {
	if (temps.channel == 0) {
		temps.temp0F = convert_adc_to_fahrenheit(ADC+TEMP_OFFSET);		// Get temp of sensor 1
	}
	else if (temps.channel == 1) {
		temps.temp1F = convert_adc_to_fahrenheit(ADC+TEMP_OFFSET);		// Get temp of sensor 2

		temps.tempFinal = (temps.temp0F + temps.temp1F) / 2.0;			// Average the temp of both sensors
	}

	adc_disable_int();													// Disable ADC Conversion Complete interrupt
}

/* Clean up the string from terminal to rid of return chars */
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

/* Pour a given recipe */
void pour_recipe(int recipe) {
	int i;
	printf("\n----------\nPOURING RECIPE\n----------\n");
	printf("Make sure there is a glass ready\n\n");
	printf("Pouring in 5...");
	for (i = 4; i > 0; i--) {
		_delay_ms(500);
		printf("%d...", i);
	}
	printf("\n\nPouring %1.2f ounces of %s\n", recipes[recipe]->AmountOne, recipes[recipe]->IngredientOne);
	pouring_length = (recipes[recipe]->AmountOne * OUNCE);			// Pour first ingredient
	enable_motor_timer(1);											// Enable timer for motor 1

	printf("Pouring %1.2f ounces of %s\n", recipes[recipe]->AmountTwo, recipes[recipe]->IngredientTwo);
	pouring_length = (recipes[recipe]->AmountTwo * OUNCE);			// Pour second ingredient
	enable_motor_timer(2);											// Enable timer for motor 2

	printf("Pouring %1.2f ounces of %s\n", recipes[recipe]->AmountThree, recipes[recipe]->IngredientThree);
	pouring_length = (recipes[recipe]->AmountThree * OUNCE);		// Pour third ingredient
	enable_motor_timer(3);											// Enable timer for motor 3
	
	printf("Pouring %1.2f ounces of %s\n", recipes[recipe]->AmountFour, recipes[recipe]->IngredientFour);
	pouring_length = (recipes[recipe]->AmountFour * OUNCE);			// Pour fourth ingredient
	enable_motor_timer(4);											// Enable timer for motor 4
}

/* Let user manage the recipe and update information pertaining to it */
void manage_recipe(int recipe) {
	unsigned char choice;

	printf("\n--------------------\n");
	dumpRecipeState(recipes[recipe]);
	printf("\n--------------------\n");

	while (1) {
		printf("\n0. Display recipe's information\n");
		printf("1. Update recipe name\n");
		printf("2. Update ingredient one\n");
		printf("3. Update ingredient two\n");
		printf("4. Update ingredient three\n");
		printf("5. Update ingredient four\n");
		printf("6. Update glass type\n");
		printf("7. --POUR RECIPE--\n");
		printf("8. Back\n");
		printf("\nSelect an option (1-8): ");

		scanf("%c", &choice);							// Get user input

		if (choice == '0') {
			printf("\n--------------------\n");
			dumpRecipeState(recipes[recipe]);			// Show recipe info
			printf("\n--------------------\n");
		}
		else if (choice == '1') {
			update_recipe_name(recipe);					// Go update the name of the recipe
		}
		else if (choice == '2') {
			update_recipe_ingredient(recipe, 1);		// Go update ingredient 1 of the recipe
		}
		else if (choice == '3') {
			update_recipe_ingredient(recipe, 2);		// Go update ingredient 2 of the recipe
		}
		else if (choice == '4') {
			update_recipe_ingredient(recipe, 3);		// Go update ingredient 3 of the recipe
		}
		else if (choice == '5') {
			update_recipe_ingredient(recipe, 4);		// Go update ingredient 4 of the recipe
		}
		else if (choice == '6') {
			update_recipe_glass(recipe);				// Go update the glass type of the recipe
		}
		else if (choice == '7') {
			pour_recipe(recipe);						// Pour the chosen recipe
		}
		else if (choice == '8') {
			display_recipes();							// Go back and display all recipes again
		}
	}
}

/* Display all available recipes */
void display_recipes(void) {
	int i;
	unsigned char choice;
	/* Show info for each recipe */
	for (i = 0; i < NUMBER_OF_RECIPES; i++) {
		printf("--------------------\nRecipe %d\n--------------------\n", i+1);
		dumpRecipeState(recipes[i]);
	}
	while(1) {
		printf("\nPick a recipe (1-5) or type 'b' to return home: ");
		scanf("%c", &choice);

		/* Manage the info of a chosen recipe */
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
				welcome_screen();
			default:
				printf("\nInvalid option!");
		}
	}
}

/* Starting page presented to the user */
void welcome_screen(void) {
	char choice = '0';

	printf("\n\nMr. Pour!  The automated beverage creator!\n--------------------\n\n");
	printf("Created by: Chris Eustis\n            Slater Claudel\n\n");
	printf("Menu: \n");
	printf("1. View available recipes \n");
	printf("2. Set internal temperature \n");
	printf("Please select any option above by entering the item number: ");

	while (1) {
		scanf("%c", &choice);
		// fgets(&choice, 1, stdin);

		if (choice == '1') {
			display_recipes();				// Show all recipes available
		}
		else if (choice == '2') {
			set_temperature();				// Manage the internal temperature
		}
		else {
			printf("Invalid value. Please select any option above by entering the item number: ");
		}
	}
}

/* Main function */
int main(void) {
	init_motors();							// Initialize motors - ensure they are not on
	init_motors_timer();					// Set up timer for controlling liquids
	init_usart(BAUDRATE, TRANSMIT_RATE, DATA_BITS, STOP_BITS, PARITY_BITS);			// Initialize usart
	init_adc(REFERENCE, PRESCALER);			// Initialize ADC
	init_recipes();							// Get recipes from eeprom

	stdin = stdout = &usart0_str;			// Set standard streams to serial

	temps.user_defined_temp = 50;			// Start off with internal temp set to 50 F

	sei();									// Enable global interrupts

	enable_temp_sensor_timer_interrupt();	// Enable timer3 overflow interrupt for reading temp sensors

	welcome_screen();						// Start off with the welcome screen

	// TODO: exit/standy-by/power-off?
	return 1;
}