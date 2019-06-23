#include "Cookbook.h"

#include "Ingredient.h"
#include "Recipe.h"


Cookbook::Cookbook()
: count_ingredients(0)
, ingredients(nullptr)
, count_recipes(0)
, recipes(nullptr)
{
  
}

void Cookbook::load()
{
  // in the future, this will actually load in the data from EEPROM
//  count_ingredients = 3;
//  ingredients = new Ingredient*[count_ingredients];
//  ingredients[0] = new Ingredient(80, 11340, 97, "Vodka", 1);
//  ingredients[1] = new Ingredient(40, 11340, 115, "Kahlua", 2);
//  ingredients[2] = new Ingredient(0, 473, 101, "Cream", 3);
//  
//  count_recipes = 1;
//  recipes = new Recipe*[count_recipes];
//  
//  uint8_t count_cols = 3;
//  uint8_t count_rows = 3;
//  uint8_t** recipe = {{15, 15, 30}, {0, 15, 30}, {0, 0, 30}};
//  
//  recipes[0] = new Recipe(this, count_cols, count_rows, recipe);
  
};
