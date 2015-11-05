#define F_CPU 16000000UL

// Library files
#include "../Library/usart.c"
#include "../Library/serial.c"

// Standard headers
#include <avr/io.h>
#include <stdio.h>
#include <avr/eeprom.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

// Set baud for serial communication
#define BAUD 9600
#define BAUDRATE ((F_CPU) / (BAUD * 16UL) - 1)

// Set usart info
#define TRANSMIT_RATE 1
#define DATA_BITS 8
#define STOP_BITS 1
#define PARITY_BITS 0

// Define recipe information
#define RECIPE_TOTAL_LENGTH 118
#define RECIPE_NAME_LENGTH 17
#define RECIPE_AMOUNT_LENGTH 4

// Define recipes to store in EEPROMs
#define RECIPE_ONE_NAME			"Recipe One"
#define RECIPE_ONE_ING_1		"Ingredient One"
#define RECIPE_ONE_AMT_1		"1"
#define RECIPE_ONE_ING_2		"Ingredient Two"
#define RECIPE_ONE_AMT_2		"2"
#define RECIPE_ONE_ING_3		"Ingredient Three"
#define RECIPE_ONE_AMT_3		"3"
#define RECIPE_ONE_ING_4		"Ingredient Four"
#define RECIPE_ONE_AMT_4		"4"
#define RECIPE_ONE_ING_5		"Ingredient Five"
#define RECIPE_ONE_AMT_5		"5"
#define RECIPE_ONE_GLASS_TYPE	"Highball Glass"

uint8_t EEMEM ee_odd_or_even;
char EEMEM ee_total_recipe[RECIPE_TOTAL_LENGTH];

int main(int argc, const char *argv[])
{
    // Point stdout to serial stream (for testing to see adc value)
    // stdout = &mystdout;

    // char *recipe = malloc(RECIPE_TOTAL_LENGTH);

	// Initialize usart
	// init_usart(BAUDRATE, TRANSMIT_RATE, DATA_BITS, STOP_BITS, PARITY_BITS);


	char recipe1[RECIPE_TOTAL_LENGTH] = "Recipe One       Rec1Ing1         1   Rec1Ing2         1.2 Rec1Ing3         1.3 Rec1Ing4         1.4 Highball         ";
    eeprom_write_block ((void*)&recipe1, (const void*)0x00, RECIPE_TOTAL_LENGTH);

	char recipe2[RECIPE_TOTAL_LENGTH] = "Recipe Two       Rec2Ing1         2   Rec2Ing2         2.2 Rec2Ing3         2.3 Rec2Ing4         1.4 Pilsner          ";
    eeprom_write_block ((void*)&recipe2, (const void*)0x76, RECIPE_TOTAL_LENGTH);

	char recipe3[RECIPE_TOTAL_LENGTH] = "Recipe Three     Rec3Ing1         3   Rec3Ing2         3.2 Rec3Ing3         3.3 Rec3Ing4         3.4 Thistle          ";
    eeprom_write_block ((void*)&recipe3, (const void*)0xEB, RECIPE_TOTAL_LENGTH);

	char recipe4[RECIPE_TOTAL_LENGTH] = "Recipe Four      Rec4Ing1         4   Rec4Ing2         4.2 Rec4Ing3         4.3 Rec4Ing4         4.4 Beer Flute       ";
    eeprom_write_block ((void*)&recipe4, (const void*)0x160, RECIPE_TOTAL_LENGTH);

	char recipe5[RECIPE_TOTAL_LENGTH] = "Recipe Five      Rec5Ing1         5   Rec5Ing2         5.2 Rec5Ing3         5.3 Rec5Ing4         5.4 Snifter          ";
    eeprom_write_block ((void*)&recipe5, (const void*)0x1D5, RECIPE_TOTAL_LENGTH);

	// int length = strlen(RECIPE_ONE_NAME);

	// if (length < RECIPE_NAME_LENGTH)
	// {
	// 	int i;
	// 	char offset[RECIPE_NAME_LENGTH-length];
	// 	for(i = 0; i < length; i++)
	// 	{
	// 		strcat(offset, " ");
	// 	}
	// 	strcat(RECIPE_ONE_NAME, offset);
	// }

	// // char recipe_name[RECIPE_NAME_LENGTH] = "Recipe One";
	// length = strlen()

	// // char recipe_ing_1[RECIPE_NAME_LENGTH] = "Ingredient One";
	// strcat(recipe, RECIPE_ONE_ING_1);
	// // char recipe_amt_1[RECIPE_NAME_LENGTH] = "1";
	// strcat(recipe, RECIPE_ONE_AMT_1);

	// // char recipe_ing_2[RECIPE_NAME_LENGTH] = "Ingredient Two";
	// strcat(recipe, RECIPE_ONE_ING_2);
	// // char recipe_amt_2[RECIPE_NAME_LENGTH] = ".5";
	// strcat(recipe, RECIPE_ONE_AMT_2);
	
	// // char recipe_ing_3[RECIPE_NAME_LENGTH] = "Ingredient Three";
	// strcat(recipe, RECIPE_ONE_ING_3);
	// // char recipe_amt_3[RECIPE_NAME_LENGTH] = "2";
	// strcat(recipe, RECIPE_ONE_AMT_3);
	
	// // char recipe_ing_4[RECIPE_NAME_LENGTH] = "Ingredient Four";
	// strcat(recipe, RECIPE_ONE_ING_4);
	// // char recipe_amt_4[RECIPE_NAME_LENGTH] = "4";
	// strcat(recipe, RECIPE_ONE_AMT_4);
	
	// // char recipe_ing_5[RECIPE_NAME_LENGTH] = "Ingredient Five";
	// strcat(recipe, RECIPE_ONE_ING_5);
	// // char recipe_amt_5[RECIPE_NAME_LENGTH] = ".5";
	// strcat(recipe, RECIPE_ONE_AMT_5);

	// // char recipe_glass_type[RECIPE_NAME_LENGTH] = "Highball";
	// strcat(recipe, RECIPE_ONE_GLASS_TYPE);



 
    // eeprom_read_block((void*)&odd_or_even_string, (const void*)&ee_total_recipe, RECIPE_TOTAL_LENGTH);

    // printf("ReadBlock: %s\n", odd_or_even_string);

	return 0;
}