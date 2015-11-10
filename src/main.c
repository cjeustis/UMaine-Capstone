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
#include "lib/usart/usart.c"
#include "lib/serial/serial.c"
// #include "../Library/adc.c"
#include "lib/recipes/recipes_object.c"
#include "lib/motors/motors.c"

// Set baud for serial communication
#define BAUD 9600
#define BAUDRATE ((F_CPU) / (BAUD * 16UL) - 1)

// Set usart info
#define TRANSMIT_RATE 1
#define DATA_BITS 8
#define STOP_BITS 1
#define PARITY_BITS 0

#define USER_NAME_LENGTH 17
#define USER_AMOUNT_LENGTH 4

/* PROTOTYPES */
void init_timer(void);
void enable_motor_timer(int motor);
char* clean_string(int size, char string[]);
void pour_recipe(int recipe);
void update_recipe_name(int recipe);
void update_recipe_glass(int recipe);
void update_recipe_ingredient(int recipe, int ingredient);
void manage_recipe(int recipe);
void display_recipes(void);
int set_temperature(void);
unsigned char welcome_screen(void);
/* END PROTOTYPES */

// static uint8_t user_temperature;
volatile float tot_overflow;
// volatile float motor1_overflow;
// volatile float motor2_overflow;
// volatile float motor3_overflow;
// volatile float motor4_overflow;
static float pouring_length;

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

void enable_motor_timer(int motor) {
	if(pouring_length != 0) {
		motor_on(motor);

		sei();

		// Wait to be done pouring liquid
		while (tot_overflow < pouring_length);

		motor_off(motor);
		tot_overflow = 0;
		cli();
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

void update_recipe_name(int recipe) {
	printf("\n----------\nUpdate Recipe Name\n----------\n");
	char temp[USER_NAME_LENGTH];
	printf("Current name of the recipe: %s\n", recipes[recipe]->RecipeName);
	printf("New name of the recipe: ");
	fgets(temp, USER_NAME_LENGTH, stdin);
	memcpy(recipes[recipe]->RecipeName, clean_string(USER_NAME_LENGTH, temp), USER_NAME_LENGTH);

	save_recipe_to_eeprom(recipe);
	deleteRecipe(recipes[recipe]);
	recipes[recipe] = get_recipe_from_eeprom(set_recipe_eeprom_address(recipe));
	convert_amount_to_float(recipe);
	printf("\n--------------------\nRecipe updated\n--------------------\n");
	dumpRecipeState(recipes[recipe]);
}

void update_recipe_glass(int recipe) {
	printf("\n----------\nUpdate Glass Type\n----------\n");
	char temp[USER_NAME_LENGTH];
	printf("Current type of glass: %s\n", recipes[recipe]->GlassType);
	printf("New glass type: ");
	fgets(temp, USER_NAME_LENGTH, stdin);
	memcpy(recipes[recipe]->GlassType, clean_string(USER_NAME_LENGTH, temp), USER_NAME_LENGTH);
	// What happens to characters that are over length?

	save_recipe_to_eeprom(recipe);
	deleteRecipe(recipes[recipe]);
	recipes[recipe] = get_recipe_from_eeprom(set_recipe_eeprom_address(recipe));
	convert_amount_to_float(recipe);
	printf("\n--------------------\nRecipe updated\n--------------------\n");
	dumpRecipeState(recipes[recipe]);
}

void update_recipe_ingredient(int recipe, int ingredient) {
	printf("\n----------\nUpdate Recipe Ingredient\n----------\n");
	char temp_name[USER_NAME_LENGTH];
	char temp_amnt[USER_AMOUNT_LENGTH];

	printf("Enter the name of the ingredient: ");
	fgets(temp_name, USER_NAME_LENGTH, stdin);

	printf("Enter amount of ingredient (0-8 ounces): ");
	fgets(temp_amnt, USER_AMOUNT_LENGTH, stdin);


	if (ingredient == 1) {
		memcpy(recipes[recipe]->IngredientOne, clean_string(USER_NAME_LENGTH, temp_name), USER_NAME_LENGTH);
		memcpy(recipes[recipe]->IngredientOne_amount, clean_string(USER_AMOUNT_LENGTH, temp_amnt), USER_AMOUNT_LENGTH);
	}
	else if (ingredient == 2) {
		memcpy(recipes[recipe]->IngredientTwo, clean_string(USER_NAME_LENGTH, temp_name), USER_NAME_LENGTH);
		memcpy(recipes[recipe]->IngredientTwo_amount, clean_string(USER_AMOUNT_LENGTH, temp_amnt), USER_AMOUNT_LENGTH);
	}
	else if (ingredient == 3) {
		memcpy(recipes[recipe]->IngredientThree, clean_string(USER_NAME_LENGTH, temp_name), USER_NAME_LENGTH);
		memcpy(recipes[recipe]->IngredientThree_amount, clean_string(USER_AMOUNT_LENGTH, temp_amnt), USER_AMOUNT_LENGTH);
	}
	else if (ingredient == 4) {
		memcpy(recipes[recipe]->IngredientFour, clean_string(USER_NAME_LENGTH, temp_name), USER_NAME_LENGTH);
		memcpy(recipes[recipe]->IngredientFour_amount, clean_string(USER_AMOUNT_LENGTH, temp_amnt), USER_AMOUNT_LENGTH);
	}

	save_recipe_to_eeprom(recipe);
	deleteRecipe(recipes[recipe]);
	recipes[recipe] = get_recipe_from_eeprom(set_recipe_eeprom_address(recipe));
	convert_amount_to_float(recipe);
	printf("\n--------------------\nRecipe updated\n--------------------\n");
	dumpRecipeState(recipes[recipe]);
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
		printf("\nPick a recipe (1-5): ");
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
		}
	}
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
	return 1;
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

	return 1;
}

int main(void) {
	init_motors();
	init_timer();
	// Set standard streams to serial
	stdin = stdout = &usart0_str;

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