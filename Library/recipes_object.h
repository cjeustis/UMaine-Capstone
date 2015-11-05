typedef struct recipes * Recipes;
Recipes newRecipes();
// void setRecipe(Recipes recipe, char, char, char, char, char, char, char, char, char, char);
void dumpRecipeState(Recipes);      /* dumps debug contents of Recipes to stdout */
void deleteRecipe(Recipes);