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
// #include "lib/pwm/pwm.c"

/* INTERRUPT PRIORITY:
*  16. Timer/Counter1 Overflow
*  25. ADC Convertsion Complete
*  35. Timer/Counter3 Overflow
*/

/* Timer/Counter 1 Overflow Interrupt (16-bit timer) */
ISR(TIMER1_OVF_vect)
{
	motors_overflow_count++;
}

/* Timer/Counter 3 Overflow Interrupt (16-bit timer) */
ISR(TIMER3_OVF_vect)
{
	temp_overflow_count++;

	/* Check the temp every ~10 seconds */
	if (temp_overflow_count >= TEN_SECONDS) {
		get_internal_temp();

		if (temps.tempFinal < 1) {
			get_internal_temp();
		}

		if (temps.tempFinal > temps.user_defined_temp) {
			if (!temps.isCooling) {
				temps.isCooling = true;
				PORTD |= _BV(PD7);
			}
		} else {
			if (temps.tempFinal <= temps.user_defined_temp-1.5) {
				if (temps.isCooling) {
					temps.isCooling = false;
					PORTD &= ~_BV(PD7);
				}
			}
		}
		temp_overflow_count = 0;
	}
}

/* ADC Conversion Interrupt */
ISR(ADC_vect)
{
	if (temps.channel == 0) {
		temps.temp0F = eval_temp(ADC);
	}
	else if (temps.channel == 1) {
		temps.temp1F = eval_temp(ADC);

		temps.tempFinal = (temps.temp0F + temps.temp1F) / 2.0;
	}

	adc_disable_int();
}

/* Clean up the string from terminal to rid of return chars */
char* clean_string(int size, char string[])
{
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
void pour_recipe(int recipe)
{
	int i;
	printf("\n----------\nPOURING RECIPE\n----------\n");
	printf("Make sure there is a glass ready\n\n");
	for (i = 5; i >= 0; i--) {
		_delay_ms(500);
		printf("\rPouring in %d...", i);
		fflush(stdout);
	}

	pouring_length = (recipes[recipe]->AmountOne * OUNCE);
	if (pouring_length != 0) {	
		pouring_length += MOTOR1_OFFSET;
		printf("\n\nPouring %1.2f ounces of %s\n\n", (double)recipes[recipe]->AmountOne, recipes[recipe]->IngredientOne);
		enable_motor_timer(1);
		_delay_ms(100);
	}

	pouring_length = (recipes[recipe]->AmountTwo * OUNCE);
	if (pouring_length != 0) {
		pouring_length += MOTOR2_OFFSET;
		printf("\nPouring %1.2f ounces of %s\n\n", (double)recipes[recipe]->AmountTwo, recipes[recipe]->IngredientTwo);
		enable_motor_timer(2);
		_delay_ms(100);
	}

	pouring_length = (recipes[recipe]->AmountThree * OUNCE);
	if (pouring_length != 0) {
		pouring_length += MOTOR3_OFFSET;
		printf("\nPouring %1.2f ounces of %s\n\n", (double)recipes[recipe]->AmountThree, recipes[recipe]->IngredientThree);
		enable_motor_timer(3);
		_delay_ms(100);
	}

	pouring_length = (recipes[recipe]->AmountFour * OUNCE);
	if (pouring_length != 0) {
		pouring_length += MOTOR4_OFFSET;
		printf("\nPouring %1.2f ounces of %s\n\n", (double)recipes[recipe]->AmountFour, recipes[recipe]->IngredientFour);
		enable_motor_timer(4);
	}
}

/* Let user manage the recipe and update information pertaining to it */
void manage_recipe(int recipe)
{
	char choice[1];

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

		fgets(choice, 2, stdin);

		if (choice[0] == '0') {
			printf("\n--------------------\n");
			dumpRecipeState(recipes[recipe]);
			printf("\n--------------------\n");
		}
		else if (choice[0] == '1') {
			update_recipe_name(recipe);
		}
		else if (choice[0] == '2') {
			update_recipe_ingredient(recipe, 1);
		}
		else if (choice[0] == '3') {
			update_recipe_ingredient(recipe, 2);
		}
		else if (choice[0] == '4') {
			update_recipe_ingredient(recipe, 3);
		}
		else if (choice[0] == '5') {
			update_recipe_ingredient(recipe, 4);
		}
		else if (choice[0] == '6') {
			update_recipe_glass(recipe);
		}
		else if (choice[0] == '7') {
			pour_recipe(recipe);
		}
		else if (choice[0] == '8') {
			display_recipes();
		}
	}
}

/* Display all available recipes */
void display_recipes(void)
{
	int i;
	char choice[0];
	/* Show info for each recipe */
	for (i = 0; i < NUMBER_OF_RECIPES; i++) {
		printf("--------------------\nRecipe %d\n--------------------\n", i+1);
		dumpRecipeState(recipes[i]);
	}
	while(1) {
		printf("\nPick a recipe (1-5) or type 'b' to return home: ");
		fgets(choice, 2, stdin);

		/* Manage the info of a chosen recipe */
		switch(choice[0]) {
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
void welcome_screen(void)
{
	printf("\e[1;1H\e[2J");
	char choice[1];
	printf("Mr. Pour!  The automated beverage creator!\n--------------------\n\n");
	printf("Created by: Chris Eustis\n            Slater Claudel\n\n");
	printf("Menu: \n");
	printf("1. View available recipes \n");
	printf("2. Set internal temperature \n");

	while (1) {
		printf("Please select any option above by entering the item number: ");
		fgets(choice, 2, stdin);

		if (choice[0] == '1') {
			display_recipes();
		}
		else if (choice[0] == '2') {
			set_temperature();
		}
		else {
			printf("\nInvalid value.\n");
		}
	}
}

void init_peltier_port(void)
{
	DDRD = _BV(PD7);
	PORTD &= ~_BV(PD7);
}

/* Main function */
int main(void)
{
	// init_pwm();
	init_peltier_port();
	init_motors();
	init_motors_timer();
	init_usart(BAUDRATE, TRANSMIT_RATE, DATA_BITS, STOP_BITS, PARITY_BITS);
	init_adc(REFERENCE, PRESCALER);
	init_recipes();

	stdin = stdout = &usart0_str;

	temps.user_defined_temp = 35;
	temps.isCooling = false;

	sei();

	enable_temp_sensor_timer_interrupt();

	welcome_screen();

	return 1;
}