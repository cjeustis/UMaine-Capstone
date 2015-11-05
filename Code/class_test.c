// Library files
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../Library/recipes_object.c"

// Set baud for serial communication
#define BAUD 9600
#define BAUDRATE ((F_CPU) / (BAUD * 16UL) - 1)

// Set usart info
#define TRANSMIT_RATE 1
#define DATA_BITS 8
#define STOP_BITS 1
#define PARITY_BITS 0

// Define recipe information
#define RECIPE_TOTAL_LENGTH 117
#define RECIPE_NAME_LENGTH 17
#define RECIPE_AMOUNT_LENGTH 4



void diddle(Recipes recipe1){
	/* Perform member functions on FooOBJ.
	 * If you try these functions on a different type of object,
	 * you will get a compile-time error
	 */
	// setRecipe(recipe1, "Cocktail1", "Vodka1", "0.1", "Rum", "0.5", "Whiskey", "1.5", "Soda", "4.0", "Highball");

}

int main(){

	char test[4] = "1.5 ";
    float pre = atof(test);
    printf("PRE: %f", pre);
    char temp[3];
    int i;
    for(i = 0; i < sizeof(temp); i++) {
        temp[i] = test[i];
    }

    printf("Temp: %s\n", temp);
    float floated = atof(temp);
    printf("Floated: %f", floated);



	// Recipes recipe1;
	// recipe1=newRecipes(); /* create a new object of type "FooOBJ" */

	// dumpRecipeState(recipe1);
	// puts("Tweaking object");
	// diddle(recipe1);

	// dumpRecipeState(recipe1);
	// deleteRecipe(recipe1);

	return 0;
}