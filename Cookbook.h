#ifndef DRINK_MAKER_COOKBOOK_H
#define DRINK_MAKER_COOKBOOK_H

#include <stdint.h>

// forward declarations
class Recipe;
class Ingredient;

struct Cookbook
{
public:
  
  // get the data from EEPROM
  // for now just have set data
  Cookbook();
  
  void load();
  void save();
  
  // adds a new ingredient and re-formats all the recipes
  // in order to recognize this
  void add_ingredient(Ingredient* new_ingredient);
  
  void add_recipe(Recipe* new_recipe);
  
  uint8_t count_ingredients;
  Ingredient** ingredients;
  
  uint8_t count_recipes;
  Recipe** recipes;
};

#endif
