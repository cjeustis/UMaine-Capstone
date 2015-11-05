#define F_CPU 16000000UL

// Library files
#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <avr/eeprom.h>
#include <avr/interrupt.h>
#include "../Library/lcd/lcd.h"
#include "../Library/lcd/myutils.h"
#include "../Library/lcd/lcd.c"
#include "../Library/usart.c"
#include "../Library/serial.c"
#include "../Library/adc.c"
#include "../Library/recipes_object.c"
#include "../Library/motors.c"

// Set baud for serial communication
#define BAUD 9600
#define BAUDRATE ((F_CPU) / (BAUD * 16UL) - 1)

// Set usart info
#define TRANSMIT_RATE 1
#define DATA_BITS 8
#define STOP_BITS 1
#define PARITY_BITS 0

// Define recipe information
#define NUMBER_OF_RECIPES 5
#define RECIPE_TOTAL_LENGTH 117
#define RECIPE_NAME_LENGTH 16
#define RECIPE_AMOUNT_LENGTH 3

// static uint8_t user_temperature;
Recipes recipes[NUMBER_OF_RECIPES];		// 5 Recipes structures
volatile float tot_overflow;
static float pouring_length;

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

Recipes get_eeprom_recipe(uint8_t *location) {
	Recipes recipe;
	recipe = newRecipes();

	// Recipe name
    eeprom_read_block((void*)&recipe->RecipeName, (const void*)location, RECIPE_NAME_LENGTH);
	location += (RECIPE_NAME_LENGTH + 1);

    // ingredient one
    eeprom_read_block((void*)&recipe->IngredientOne, (const void*)location, RECIPE_NAME_LENGTH);	
	location += (RECIPE_NAME_LENGTH + 1);
    
    // ingredient one amount
    eeprom_read_block((void*)&recipe->IngredientOne_amount, (const void*)location, RECIPE_AMOUNT_LENGTH);	
	location += (RECIPE_AMOUNT_LENGTH + 1);
    
    // ingredient two
    eeprom_read_block((void*)&recipe->IngredientTwo, (const void*)location, RECIPE_NAME_LENGTH);	
	location += (RECIPE_NAME_LENGTH + 1);
    
    // ingredient two amount
    eeprom_read_block((void*)&recipe->IngredientTwo_amount, (const void*)location, RECIPE_AMOUNT_LENGTH);	
	location += (RECIPE_AMOUNT_LENGTH + 1);    

    // ingredient three
    eeprom_read_block((void*)&recipe->IngredientThree, (const void*)location, RECIPE_NAME_LENGTH);	
	location += (RECIPE_NAME_LENGTH + 1);
    
    // ingredient three amount
    eeprom_read_block((void*)&recipe->IngredientThree_amount, (const void*)location, RECIPE_AMOUNT_LENGTH);	
	location += (RECIPE_AMOUNT_LENGTH + 1);
    
	// ingredient four
    eeprom_read_block((void*)&recipe->IngredientFour, (const void*)location, RECIPE_NAME_LENGTH);	
	location += (RECIPE_NAME_LENGTH + 1);
    
    // ingredient four amount
    eeprom_read_block((void*)&recipe->IngredientFour_amount, (const void*)location, RECIPE_AMOUNT_LENGTH);	
	location += (RECIPE_AMOUNT_LENGTH + 1);

	// Glass type
    eeprom_read_block((void*)&recipe->GlassType, (const void*)location, RECIPE_NAME_LENGTH);

    // return recipe;
    return recipe;
}

uint8_t* set_recipe_eeprom_address(int recipe_number) {
	uint8_t *mem_address = 0x00;

	switch(recipe_number) {
		case 0:
			return mem_address;
		case 1:
			return (mem_address + (RECIPE_TOTAL_LENGTH + 1));
		case 2:
			return (mem_address + (RECIPE_TOTAL_LENGTH * 2) + 1);
		case 3:
			return (mem_address + (RECIPE_TOTAL_LENGTH * 3) + 1);
		case 4:
			return (mem_address + (RECIPE_TOTAL_LENGTH * 4) + 1);
		default:
			return mem_address;
	}
}

void init_recipes(void) {
	int i;
	for (i = 0; i < NUMBER_OF_RECIPES; i++) {
		recipes[i] = newRecipes();
		recipes[i] = get_eeprom_recipe(set_recipe_eeprom_address(i));
	}
	for(i = 0; i < NUMBER_OF_RECIPES; i++) {
		recipes[i]->AmountOne = atof(recipes[i]->IngredientOne_amount);
		recipes[i]->AmountTwo = atof(recipes[i]->IngredientTwo_amount);
		recipes[i]->AmountThree = atof(recipes[i]->IngredientThree_amount);
		recipes[i]->AmountFour = atof(recipes[i]->IngredientFour_amount);
	}
}

void enable_motor_timer(int motor) {
	motor_on(motor);
	sei();

	// Wait to be done pouring liquid
	while (tot_overflow < pouring_length);

	motor_off(motor);
	tot_overflow = 0;
	cli();
}

void pour_recipe(int recipe) {
	printf("You chose %s\n", recipes[recipe]->RecipeName);
	printf("Pouring %1.2f ounces of %s\n", recipes[recipe]->AmountOne, recipes[recipe]->IngredientOne);
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

void display_recipes(void) {
	int i;
	unsigned char choice;
	for (i = 0; i < NUMBER_OF_RECIPES; i++) {
		printf("Recipe %d\n", i+1);
		dumpRecipeState(recipes[i]);
	}
	printf("\nWhich recipe would you like to enjoy?\n");
	
	// while(1) {
		scanf("%c", &choice);

		switch(choice) {
			case '1':
				pour_recipe(0);
				break;
			case '2':
				pour_recipe(1);
				break;
			case '3':
				pour_recipe(2);
				break;
			case '4':
				pour_recipe(3);
				break;
			case '5':
				pour_recipe(4);
				break;
		}
	// }
}

int set_temperature(void) {
	unsigned char choice;
	printf("\n\nMr. Pour: Temperature Control!\n--------------------\n\n");
	/* DISPLAY CURRENT TEMPERATURE */
	printf("Current temperature: ");
	// printf("Desired temperature: %d\n", set_temperature);
	printf("\nWould you like to change the internal temperature?\ny = yes, n = no");

	while(1) {
		scanf("%c", &choice);
		if (choice == 'y') {
			// Change temperature
			return 0;
		}
		else if (choice == 'n') {
			return 0;
		}
	}
	return -1;
}

unsigned char welcome_screen(void) {
	unsigned char choice = '0';

	printf("\n\nMr. Pour!  The automated beverage creator!\n--------------------\n\n");
	printf("Created by: Chris Eustis\n            Slater Claudel\n\n");
	printf("Menu: \n");
	printf("1. View available recipes \n");
	printf("2. Set internal temperature \n");
	printf("Please select any option above by entering the item number: \n");

	while (1) {
		scanf("%c", &choice);

		if (choice == '1') return choice;
		else if (choice == '2') return choice;
	}

	return -1;
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

int main(void) {
	cli();
	init_motors();
	init_timer();
	// Set standard streams to serial
	stdin=stdout=&usart0_str;

	// Initialize usart
	init_usart(BAUDRATE, TRANSMIT_RATE, DATA_BITS, STOP_BITS, PARITY_BITS);
	// get recipes from eeprom
	init_recipes();

	while(1) {
		unsigned char choice = welcome_screen();

		if (choice == '1') {
			// Display recipes
			display_recipes();
		}
		else if (choice == '2') {
			set_temperature();
		}
	}

	return 0;
}