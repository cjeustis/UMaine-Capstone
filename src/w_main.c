#define F_CPU 16000000UL

#include <avr/io.h>
#include <avr/eeprom.h>
#include <stdio.h>
#include <stdbool.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdlib.h>
#include "lib/lcd/lcd.h"
#include "lib/lcd/myutils.h"
#include "lib/lcd/lcd.c"
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

#define LCD_CURRENT_TEMP 1
#define LCD_DEFINED_TEMP 2

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

/* Lazy globals */
// int LCD_VIEW;
char lcd_string[17];
float g_current_temp;
float g_defined_temp;

int isPouring;

void update_LCD_screen(int view)
{
	/* Switch view on LCD */
	if (view == LCD_CURRENT_TEMP) {
		/* Create string to display on LCD */
		sprintf(lcd_string, "     %1.1f F", (double)g_current_temp);

		/* Print update to LCD */
		LCDClear();
		LCDWriteString(" Internal Temp  ");
		LCDWriteStringXY(0, 1, lcd_string);
	} 
	if (view == LCD_DEFINED_TEMP) {
		/* Create string to display on LCD */
		sprintf(lcd_string, "     %1.1f F", (double)g_defined_temp);

		/* Print update to LCD */
		LCDClear();
		LCDWriteString("  Desired Temp");
		LCDWriteStringXY(0, 1, lcd_string);
	}
}

/* Timer/Counter 1 Overflow Interrupt (16-bit timer) */
ISR(TIMER1_OVF_vect)
{
	motors_overflow_count++;
}

/* Timer/Counter 3 Overflow Interrupt (16-bit timer) */
ISR(TIMER3_OVF_vect)
{	
	temp_overflow_count++;

	if (temp_overflow_count >= TEN_SECONDS) {
		if (!isPouring)
			update_LCD_screen(LCD_DEFINED_TEMP);
		temp_overflow_count = 0;
	}
}

void compare_temps(TempData *temps)
{
	if (temps->sensor_temp > temps->defined_temp-.5) {
		if (!temps->isCooling) {
			temps->isCooling = true;
			PORTD |= _BV(PD7);
		}
	} else {
		if (temps->sensor_temp <= temps->defined_temp-1.5) {
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
		pouring_length += MOTOR1_OFFSET;
		enable_motor_timer(1);
		_delay_ms(100);
	}

	pouring_length = (recipe->ing2 * OUNCE);
	if (pouring_length != 0) {
		pouring_length += MOTOR2_OFFSET;
		enable_motor_timer(2);
		_delay_ms(100);
	}

	pouring_length = (recipe->ing3 * OUNCE);
	if (pouring_length != 0) {
		pouring_length += MOTOR3_OFFSET;
		enable_motor_timer(3);
		_delay_ms(100);
	}

	pouring_length = (recipe->ing4 * OUNCE);
	if (pouring_length != 0) {
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

void init_lcd(void)
{
	LCDInit(LS_NONE);
	LCDClear();

	LCDWriteString("    Mr. Pour    ");
	LCDWriteStringXY(0,1," Senior Project");
}

TempData *init_temps_struct(TempData *s)
{
	s->isCooling = true;
	s->defined_temp = 35.;
	s->sensor_temp = 0.;
	return s;
}

Recipe *init_recipe_struct(Recipe *s)
{
	s->ing1 = 0.;
	s->ing2 = 0.;
	s->ing3 = 0.;
	s->ing4 = 0.;
	return s;
}

void perform_inits(void)
{	
	g_defined_temp = 35;
	g_current_temp = 0.;
	init_usart(BAUDRATE, TRANSMIT_RATE, DATA_BITS, STOP_BITS, PARITY_BITS);
	init_peltier_port();
	init_motors();
	init_motors_timer();
	init_lcd();
	sei();
}

int main(void)
{
	stdin = stdout = &usart0_str;

	perform_inits();

	/* Local variables */
	char c;
	char *ing1;
	char *ing2;
	char *ing3;
	char *ing4;
	char *sensor_temp;
	char *defined_temp;
	TempData *temps;
	Recipe *recipe;

	/* Allocate space for temp struct */
	temps = malloc(sizeof(TempData));

	/* Allocate space for recipe struct */
	recipe = malloc(sizeof(Recipe));

	/* Allocate space for holding each ingredient */
	ing1 = malloc(3 * sizeof(char));
	ing2 = malloc(3 * sizeof(char));
	ing3 = malloc(3 * sizeof(char));
	ing4 = malloc(3 * sizeof(char));

	/* Allocate space for temp data */
	sensor_temp = malloc(4 * sizeof(char));
	defined_temp = malloc(2 * sizeof(char));

	/* Check for memory allocation errors */
	if ((temps == NULL) || (recipe == NULL) || (ing1 == NULL) || (ing2 == NULL) || (ing3 == NULL) || (ing4 == NULL) || (sensor_temp == NULL) || (defined_temp == NULL)) {
		LCDClear();
		LCDWriteString("Error while alloc");
		LCDWriteStringXY(0,1,"ating memory :(");
		while(1);
	}

	/* Initialize structure values */
	init_temps_struct(temps);
	init_recipe_struct(recipe);

	/* Enable Timer3 overflow interrupt - used for changing display of LCD */
	enable_temp_sensor_timer_interrupt();

	while(1) {
		// Wait for signal from website
		scanf(" %1c", &c);

		switch(c) {
			case 'c':
			// get value from temp sensor
			// compare with set temp
			// decide whether cooler is on/off
				disable_temp_sensor_timer_interrupt();
				scanf(" %4s", sensor_temp);
				convert_temp_to_float(temps, sensor_temp, 0);
				g_current_temp = temps->sensor_temp;
				if (temps->sensor_temp >= 20 || temps->sensor_temp <= 90) {
					update_LCD_screen(LCD_CURRENT_TEMP);
					compare_temps(temps);
				}
				enable_temp_sensor_timer_interrupt();
				break;
			case 'p':
			// read the four ingredient amounts
			// turn each motor on to pour the ingredients
				disable_temp_sensor_timer_interrupt();
				isPouring = true;
				scanf(" %s", ing1);
				scanf(" %s", ing2);
				scanf(" %s", ing3);
				scanf(" %s", ing4);
				convert_recipe_to_float(recipe, ing1, ing2, ing3, ing4);

				if ((recipe->ing1 >= 0. || recipe->ing1 <= 8.) && (recipe->ing2 >= 0. || recipe->ing2 <= 8.)  && (recipe->ing3 >= 0. || recipe->ing3 <= 8.)  && (recipe->ing4 >= 0. || recipe->ing4 <= 8.)) {
					LCDClear();
					LCDWriteString("Pouring recipe..");
					
					/* Go pour the recipe */
					pour_recipe(recipe);

					update_LCD_screen(LCD_CURRENT_TEMP);
				}
				enable_temp_sensor_timer_interrupt();
				break;
			case 't':
				/* Grab updated internal temp from site */
				disable_temp_sensor_timer_interrupt();
				scanf(" %2s", defined_temp);
				convert_temp_to_float(temps, defined_temp, 1);

				/* Create string to display on LCD */
				g_defined_temp = temps->defined_temp;
				sprintf(lcd_string, "     %1.1f F", (double)g_defined_temp);
				LCDClear();
				LCDWriteString("  Temp updated");
				LCDWriteStringXY(0, 1, lcd_string);

				_delay_ms(1000);
				update_LCD_screen(LCD_CURRENT_TEMP);
				enable_temp_sensor_timer_interrupt();
				break;
		}
	}
	if (ing1) free(ing1);
	if (ing2) free(ing2);
	if (ing3) free(ing3);
	if (ing4) free(ing4);
	if (sensor_temp) free(sensor_temp);
	if (defined_temp) free(defined_temp);
	if (temps) free(temps);
	if (recipe) free(recipe);
	return 0;
}