#define F_CPU 16000000UL

// Library files
#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>
#include <avr/eeprom.h>
#include <avr/interrupt.h>
#include <util/delay.h>
// #include "../Library/lcd/lcd.h"
// #include "../Library/lcd/myutils.h"
// #include "../Library/lcd/lcd.c"
#include "lib/main.h"
#include "lib/usart/usart.c"
#include "lib/serial/serial.c"
#include "lib/adc/adc.c"
#include "lib/recipes/recipes_object.c"
#include "lib/motors/motors.c"
#include "lib/timers/timer_helper.c"
#include "lib/temp/temp_helper.c"

// Set baud for serial communication
#define BAUD 9600
#define BAUDRATE ((F_CPU) / (BAUD * 16UL) - 1)

// Set usart info
#define TRANSMIT_RATE 1
#define DATA_BITS 8
#define STOP_BITS 1
#define PARITY_BITS 0

// Set adc info
#define REFERENCE 0
#define PRESCALER 128

// static uint8_t user_temperature;
// volatile float motor1_overflow;
// volatile float motor2_overflow;
// volatile float motor3_overflow;
// volatile float motor4_overflow;

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
	// Pour first ingredient
	pouring_length = (recipes[recipe]->AmountOne * OUNCE);
	enable_motor_timer(1);

	printf("Pouring %1.2f ounces of %s\n", recipes[recipe]->AmountTwo, recipes[recipe]->IngredientTwo);
	// Pour first ingredient
	pouring_length = (recipes[recipe]->AmountTwo * OUNCE);
	enable_motor_timer(2);

	printf("Pouring %1.2f ounces of %s\n", recipes[recipe]->AmountThree, recipes[recipe]->IngredientThree);
	// Pour first ingredient
	pouring_length = (recipes[recipe]->AmountThree * OUNCE);
	enable_motor_timer(3);
	
	printf("Pouring %1.2f ounces of %s\n", recipes[recipe]->AmountFour, recipes[recipe]->IngredientFour);
	// Pour first ingredient
	pouring_length = (recipes[recipe]->AmountFour * OUNCE);
	enable_motor_timer(4);
}

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

		scanf("%c", &choice);

		if (choice == '0') {
			printf("\n--------------------\n");
			dumpRecipeState(recipes[recipe]);
			printf("\n--------------------\n");
		}
		else if (choice == '1') {
			update_recipe_name(recipe);
		}
		else if (choice == '2') {
			update_recipe_ingredient(recipe, 1);
		}
		else if (choice == '3') {
			update_recipe_ingredient(recipe, 2);
		}
		else if (choice == '4') {
			update_recipe_ingredient(recipe, 3);
		}
		else if (choice == '5') {
			update_recipe_ingredient(recipe, 4);
		}
		else if (choice == '6') {
			update_recipe_glass(recipe);
		}
		else if (choice == '7') {
			pour_recipe(recipe);
		}
		else if (choice == '8') {
			display_recipes();
		}
	}
}

void display_recipes(void) {
	int i;
	unsigned char choice;
	for (i = 0; i < NUMBER_OF_RECIPES; i++) {
		printf("--------------------\nRecipe %d\n--------------------\n", i+1);
		dumpRecipeState(recipes[i]);
	}
	
	while(1) {
		printf("\nPick a recipe (1-5) or type 'b' to return home: ");
		scanf("%c", &choice);

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
			display_recipes();
		}
		else if (choice == '2') {
			set_temperature();
		}
		else {
			printf("Invalid value. Please select any option above by entering the item number: ");
		}
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

	// Start off with the welcome screen
	welcome_screen();

	// TODO: exit/standy-by/power-off?
	return 1;
}