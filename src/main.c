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

/* PROTOTYPES */
void init_timer(void);
void begin_pouring(int motor);
char* clean_string(int size, char string[]);
void pour_recipe(int recipe);
void update_recipe_name(int recipe);
void update_recipe_glass(int recipe);
void update_recipe_ingredient(int recipe, int ingredient);
void manage_recipe(int recipe);
void display_recipes(void);
void set_temperature(void);
void welcome_screen(void);
void enable_timer_interrupt(void);
void disable_timer_interrupt(void);
float convert_adc_to_fahrenheit(uint16_t adc);
void get_temp_reading(void);
void update_temperature(void);
/* END PROTOTYPES */

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

void init_timer(void) {
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

void enable_timer_interrupt(void) {
	// Enable overflow interrupt
	TIMSK1 = (1<<TOIE1);
}

void disable_timer_interrupt(void) {
	// Disable overflow interrupt
	TIMSK1 &= ~(1<<TOIE1);
}

void begin_pouring(int motor) {
	if(pouring_length != 0) {
		motor_on(motor);

		enable_timer_interrupt();

		// Wait to be done pouring liquid
		while (tot_overflow < pouring_length);

		motor_off(motor);
		tot_overflow = 0;

		disable_timer_interrupt();
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
	printf("\n\nPouring %1.2f ounces of %s\n\n", recipes[recipe]->AmountOne, recipes[recipe]->IngredientOne);
	// Pour first ingredient
	pouring_length = (recipes[recipe]->AmountOne * OUNCE);
	begin_pouring(1);

	printf("Pouring %1.2f ounces of %s\n", recipes[recipe]->AmountTwo, recipes[recipe]->IngredientTwo);
	// Pour first ingredient
	pouring_length = (recipes[recipe]->AmountTwo * OUNCE);
	begin_pouring(2);

	printf("Pouring %1.2f ounces of %s\n", recipes[recipe]->AmountThree, recipes[recipe]->IngredientThree);
	// Pour first ingredient
	pouring_length = (recipes[recipe]->AmountThree * OUNCE);
	begin_pouring(3);
	
	printf("Pouring %1.2f ounces of %s\n", recipes[recipe]->AmountFour, recipes[recipe]->IngredientFour);
	// Pour first ingredient
	pouring_length = (recipes[recipe]->AmountFour * OUNCE);
	begin_pouring(4);
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
	char choice;

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
		fgets(&choice, 1, stdin);

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
	char choice;
	for (i = 0; i < NUMBER_OF_RECIPES; i++) {
		printf("--------------------\nRecipe %d\n--------------------\n", i+1);
		dumpRecipeState(recipes[i]);
		// getchar();
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

float convert_adc_to_fahrenheit(uint16_t adc) {
	float c, v;
	/* Convert adc to voltage value */
	v = adc * 5.0;
	v = v / 1024.0;

	/* Convert voltage to celcius */
	c = (v - 0.5) * 10.0;

	/* Convert celcius to farenheit */
	return (c * 9.0 / 5.0) + 32.0;
}

void get_temp_reading(void) {
	// Start ADC0 conversion
	adc_enable_int();
	temps.channel = 0;
	start_adc(0);
	_delay_ms(10);

	// Start ADC1 conversion
	adc_enable_int();
	temps.channel = 1;
	_delay_ms(10);
	start_adc(1);
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
	if (temps.tempFinal == 0) {
		get_temp_reading();
	}
	/* DISPLAY CURRENT TEMPERATURE */
	printf("Current temperature: %1.2f\n", temps.tempFinal);
	// printf("Desired temperature: %d\n", set_temperature);
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
	init_timer();

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
	return 0;
}