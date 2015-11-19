#include "recipes_object.h"

/* Array to hold the recipes */
Recipes recipes[NUMBER_OF_RECIPES];

/* Struct for holding information about a particular recipe */
struct recipes {
	char RecipeName[RECIPE_NAME_LENGTH];
	char IngredientOne[RECIPE_NAME_LENGTH];
	char IngredientOne_amount[RECIPE_AMOUNT_LENGTH];
	float AmountOne;
	char IngredientTwo[RECIPE_NAME_LENGTH];
	char IngredientTwo_amount[RECIPE_AMOUNT_LENGTH];
	float AmountTwo;
	char IngredientThree[RECIPE_NAME_LENGTH];
	char IngredientThree_amount[RECIPE_AMOUNT_LENGTH];
	float AmountThree;
	char IngredientFour[RECIPE_NAME_LENGTH];
	char IngredientFour_amount[RECIPE_AMOUNT_LENGTH];
	float AmountFour;
	char GlassType[RECIPE_NAME_LENGTH];
};

/* Creates a new recipe struct */
Recipes newRecipes(){
	Recipes recipe = (Recipes)malloc(sizeof(struct recipes));			// Allocate memory for the struct
	memset(recipe, 0, sizeof(struct recipes));							// Initialize the struct to 0s
	return recipe;
}

/* Prints out all info about recipe */
void dumpRecipeState(Recipes recipe){
	if(recipe==NULL) return;
	printf("Recipe name: %s\n", recipe->RecipeName);
	printf("Ingredient One: %s\n", recipe->IngredientOne);
	printf("Ingredient One Amount: %f\n", recipe->AmountOne);
	printf("Ingredient Two: %s\n", recipe->IngredientTwo);
	printf("Ingredient Two Amount: %f\n", recipe->AmountTwo);
	printf("Ingredient Three: %s\n", recipe->IngredientThree);
	printf("Ingredient Three Amount: %f\n", recipe->AmountThree);
	printf("Ingredient Four: %s\n", recipe->IngredientFour);
	printf("Ingredient Four Amount: %f\n", recipe->AmountFour);
	printf("Glass Type: %s\n", recipe->GlassType);
}

/* Frees memory allocated for recipe */
void deleteRecipe(Recipes recipe){
	free(recipe);
}

/* Set location of eeprom to look at for specific recipe */
int set_recipe_eeprom_address(int recipe_number) {
	int eeprom_memory_location = 0;
	switch(recipe_number) {
		case 0:
			return eeprom_memory_location;
		case 1:
			return eeprom_memory_location + RECIPE_TOTAL_LENGTH;
		case 2:
			return eeprom_memory_location + (RECIPE_TOTAL_LENGTH * 2);
		case 3:
			return eeprom_memory_location + (RECIPE_TOTAL_LENGTH * 3);
		case 4:
			return eeprom_memory_location + (RECIPE_TOTAL_LENGTH * 4);
		default:
			return eeprom_memory_location;
	}
}

/* Create structs for each recipe and fill them with eeprom data */
void init_recipes(void) {
	int i;
	for (i = 0; i < NUMBER_OF_RECIPES; i++) {
		recipes[i] = newRecipes();					// Create new recipe struct
		recipes[i] = get_recipe_from_eeprom(set_recipe_eeprom_address(i));		// Fill new recipe struct with data from EEPROM
	}
	for(i = 0; i < NUMBER_OF_RECIPES; i++) {
		convert_amount_to_float(i);												// Convert recipe amounts to a usable float value
	}
}

/* Converts the recipe's ingredient's amounts from char to float to use when dispensing liquids */
void convert_amount_to_float(int recipe) {
	recipes[recipe]->AmountOne = atof(recipes[recipe]->IngredientOne_amount);
	recipes[recipe]->AmountTwo = atof(recipes[recipe]->IngredientTwo_amount);
	recipes[recipe]->AmountThree = atof(recipes[recipe]->IngredientThree_amount);
	recipes[recipe]->AmountFour = atof(recipes[recipe]->IngredientFour_amount);
}

/* Read eeprom and save recipe information into struct */
Recipes get_recipe_from_eeprom(int location) {
	Recipes recipe;
	recipe = newRecipes();

    eeprom_read_block((void*)&recipe->RecipeName, (const void*)location, RECIPE_NAME_LENGTH);						// Get recipe name from memory
	location += RECIPE_NAME_LENGTH;
    eeprom_read_block((void*)&recipe->IngredientOne, (const void*)location, RECIPE_NAME_LENGTH);			    	// Get ingredient one from memory
	location += RECIPE_NAME_LENGTH;
    eeprom_read_block((void*)&recipe->IngredientOne_amount, (const void*)location, RECIPE_AMOUNT_LENGTH);	    	// Get ingredient one amount from memory
	location += RECIPE_AMOUNT_LENGTH+1;
    eeprom_read_block((void*)&recipe->IngredientTwo, (const void*)location, RECIPE_NAME_LENGTH);			    	// Get ingredient two from memory
	location += RECIPE_NAME_LENGTH;
    eeprom_read_block((void*)&recipe->IngredientTwo_amount, (const void*)location, RECIPE_AMOUNT_LENGTH);	    	// Get ingredient two amount from memory
	location += RECIPE_AMOUNT_LENGTH+1;
    eeprom_read_block((void*)&recipe->IngredientThree, (const void*)location, RECIPE_NAME_LENGTH);			    	// Get ingredient three from memory
	location += RECIPE_NAME_LENGTH;
    eeprom_read_block((void*)&recipe->IngredientThree_amount, (const void*)location, RECIPE_AMOUNT_LENGTH);     	// Get ingredient three amount from memory
	location += RECIPE_AMOUNT_LENGTH+1;
    eeprom_read_block((void*)&recipe->IngredientFour, (const void*)location, RECIPE_NAME_LENGTH);					// Get ingredient four from memory
	location += RECIPE_NAME_LENGTH;
    eeprom_read_block((void*)&recipe->IngredientFour_amount, (const void*)location, RECIPE_AMOUNT_LENGTH);	    	// Get ingredient four amount from memory
	location += RECIPE_AMOUNT_LENGTH+1;
    eeprom_read_block((void*)&recipe->GlassType, (const void*)location, RECIPE_NAME_LENGTH);						// Get glass type from memory

    return recipe;
}

/* TODO: change from eeprom_write to eeprom_update */
/* Save the given recipe to EEPROM so the data persists */
void save_recipe_to_eeprom(int recipe) {
	int location = set_recipe_eeprom_address(recipe);				// Get recipe location before saving to EEPROM

    eeprom_write_block ((void*)&recipes[recipe]->RecipeName, (void*)location, RECIPE_NAME_LENGTH);					// Write ecipe name to memory
	location += RECIPE_NAME_LENGTH;
    eeprom_write_block ((void*)&recipes[recipe]->IngredientOne, (void*)location, RECIPE_NAME_LENGTH);		    	// Write ingredient one to memory
	location += RECIPE_NAME_LENGTH;
    eeprom_write_block ((void*)&recipes[recipe]->IngredientOne_amount, (void*)location, RECIPE_AMOUNT_LENGTH);  	// Write ingredient one amount to memory
	location += RECIPE_AMOUNT_LENGTH+1;
    eeprom_write_block ((void*)&recipes[recipe]->IngredientTwo, (void*)location, RECIPE_NAME_LENGTH);  				// Write ingredient two to memory
	location += RECIPE_NAME_LENGTH;    
    eeprom_write_block ((void*)&recipes[recipe]->IngredientTwo_amount, (void*)location, RECIPE_AMOUNT_LENGTH);  	// Write ingredient two amount to memory to memory
	location += RECIPE_AMOUNT_LENGTH+1;
    eeprom_write_block ((void*)&recipes[recipe]->IngredientThree, (void*)location, RECIPE_NAME_LENGTH);    			// Write ingredient three to memory
	location += RECIPE_NAME_LENGTH;    
    eeprom_write_block ((void*)&recipes[recipe]->IngredientThree_amount, (void*)location, RECIPE_AMOUNT_LENGTH);	// Write ingredient three amount to memory
	location += RECIPE_AMOUNT_LENGTH+1;
    eeprom_write_block ((void*)&recipes[recipe]->IngredientFour, (void*)location, RECIPE_NAME_LENGTH);				// Write ingredient four to memory
	location += RECIPE_NAME_LENGTH;    
    eeprom_write_block ((void*)&recipes[recipe]->IngredientFour_amount, (void*)location, RECIPE_AMOUNT_LENGTH);	    // Write ingredient four amount to memory
	location += RECIPE_AMOUNT_LENGTH+1;
    eeprom_write_block ((void*)&recipes[recipe]->GlassType, (void*)location, RECIPE_NAME_LENGTH);					// Write glass type to memory
}

/* Update a given recipe's name, saving to EEPROM and displaying the updated recipe back to screen */
void update_recipe_name(int recipe) {
	char temp[USER_NAME_LENGTH];

	printf("\n----------\nUpdate Recipe Name\n----------\n");
	printf("Current name of the recipe: %s\n", recipes[recipe]->RecipeName);
	printf("New name of the recipe: ");
	fgets(temp, USER_NAME_LENGTH, stdin);				// Read in data from the terminal
	memcpy(recipes[recipe]->RecipeName, clean_string(USER_NAME_LENGTH, temp), USER_NAME_LENGTH);					// Save user input to recipe struct

	save_recipe_to_eeprom(recipe);						// Save recipe to EEPROM
	deleteRecipe(recipes[recipe]);						// Delete (free) the recipe from memory
	recipes[recipe] = get_recipe_from_eeprom(set_recipe_eeprom_address(recipe));									// Get the updated recipe from EEPROM
	convert_amount_to_float(recipe);					// Convert amounts to float values

	printf("\n--------------------\nRecipe updated\n--------------------\n");
	dumpRecipeState(recipes[recipe]);
}

/* Update a recipe's glass type, saving to EEPROM and displaying the updated recipe back to screen */
void update_recipe_glass(int recipe) {
	char temp[USER_NAME_LENGTH];

	printf("\n----------\nUpdate Glass Type\n----------\n");
	printf("Current type of glass: %s\n", recipes[recipe]->GlassType);
	printf("New glass type: ");
	fgets(temp, USER_NAME_LENGTH, stdin);				// Read in data from the terminal
	memcpy(recipes[recipe]->GlassType, clean_string(USER_NAME_LENGTH, temp), USER_NAME_LENGTH);						// Save user input to recipe struct

	save_recipe_to_eeprom(recipe);						// Save recipe to EEPROM
	deleteRecipe(recipes[recipe]);						// Delete (free) the recipe from memory
	recipes[recipe] = get_recipe_from_eeprom(set_recipe_eeprom_address(recipe));									// Get the updated recipe from EEPROM
	convert_amount_to_float(recipe);					// Convert amounts to float values

	printf("\n--------------------\nRecipe updated\n--------------------\n");
	dumpRecipeState(recipes[recipe]);
}

/* Update a recipe's ingredient information, saving to EEPROM and displaying the udpated recipe back to screen */
void update_recipe_ingredient(int recipe, int ingredient) {
	char temp_name[USER_NAME_LENGTH];
	char temp_amnt[USER_AMOUNT_LENGTH];

	printf("\n----------\nUpdate Recipe Ingredient\n----------\n");
	printf("Enter the name of the ingredient: ");
	fgets(temp_name, USER_NAME_LENGTH, stdin);					// Read in data from the terminal for ingredient name
	printf("\nEnter amount of ingredient (0-8 ounces): ");
	fgets(temp_amnt, USER_AMOUNT_LENGTH, stdin);				// Read in data from the terminal for ingredient amount

	/* Save data to recipe struct, depending on which recipe it is */
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

	save_recipe_to_eeprom(recipe);						// Save updated recipe to EEPROM
	deleteRecipe(recipes[recipe]);						// Delete (free) the recipe from memory
	recipes[recipe] = get_recipe_from_eeprom(set_recipe_eeprom_address(recipe));									// Get the updated recipe from EEPROM
	convert_amount_to_float(recipe);					// Convert amounts to float values

	printf("\n--------------------\nRecipe updated\n--------------------\n");
	dumpRecipeState(recipes[recipe]);
}