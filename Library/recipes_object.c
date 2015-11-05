#include <stdio.h>
#include "recipes_object.h"

#define RECIPE_LENGTH_NAME 17
#define RECIPE_LENGTH_AMOUNT 4

struct recipes {
	char RecipeName[RECIPE_LENGTH_NAME];
	char IngredientOne[RECIPE_LENGTH_NAME];
	char IngredientOne_amount[RECIPE_LENGTH_AMOUNT];
	float AmountOne;
	char IngredientTwo[RECIPE_LENGTH_NAME];
	char IngredientTwo_amount[RECIPE_LENGTH_AMOUNT];
	float AmountTwo;
	char IngredientThree[RECIPE_LENGTH_NAME];
	char IngredientThree_amount[RECIPE_LENGTH_AMOUNT];
	float AmountThree;
	char IngredientFour[RECIPE_LENGTH_NAME];
	char IngredientFour_amount[RECIPE_LENGTH_AMOUNT];
	float AmountFour;
	char GlassType[RECIPE_LENGTH_NAME];
	// Put memory location of recipe here
};

/* Any "class static" vars simply get declared as
static int staticvar;
 * or whatever you like here.
 * Whereas instance variables go in the struct above.
 */

Recipes newRecipes(){
	Recipes recipe=(Recipes)malloc(sizeof(struct recipes));
	bzero(recipe, sizeof(struct recipes));
	return recipe;
}

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

void deleteRecipe(Recipes recipe){
	free(recipe);
}