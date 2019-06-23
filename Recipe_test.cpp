#include "catch.hpp"

// include twice to test for header guard
#include "Recipe.h"
#include "Recipe.h"

#include "Cookbook.h"
#include "Ingredient.h"

#include <stdint.h>
#include <iostream>

TEST_CASE("Recipe", "[recipe]")
{
  Cookbook cookbook;
  
  cookbook.count_ingredients = 3;
  cookbook.ingredients = new Ingredient*[cookbook.count_ingredients];
  cookbook.ingredients[0] = new Ingredient(80, 11340, 97, "Vodka", 1);
  cookbook.ingredients[1] = new Ingredient(40, 11340, 115, "Kahlua", 2);
  cookbook.ingredients[2] = new Ingredient(0, 473, 101, "Cream", 3);
  
  cookbook.count_recipes = 1;
  cookbook.recipes = new Recipe*[cookbook.count_recipes];
  
  uint8_t count_cols = 3;
  uint8_t count_rows = 2;
  uint8_t** recipe_data = new uint8_t*[3];
  recipe_data[1] = new uint8_t[3] {15, 15, 30};
  recipe_data[0] = new uint8_t[3] {0, 15, 30};
  
  cookbook.recipes[0] = new Recipe(&cookbook, "White Russian", count_cols, count_rows, recipe_data);
  
  Recipe* recipe = cookbook.recipes[0];
  
  SECTION("default strength")
  {
    REQUIRE(recipe->get_default_strength() == 162);
  }
  
  SECTION("making the drink")
  {
    // try an above
    uint16_t* weights = recipe->get_weights_for_strength(200);
    
    REQUIRE(weights[0] == 532);
    REQUIRE(weights[1] == 630);
    REQUIRE(weights[2] == 1106);
    
    // try an in between
    weights = recipe->get_weights_for_strength(100);
    
    REQUIRE(weights[0] == 185);
    REQUIRE(weights[1] == 511);
    REQUIRE(weights[2] == 896);
    
    weights = recipe->get_weights_for_strength(150);
    
    REQUIRE(weights[2] == 896);
    
    //try one below
    weights = recipe->get_weights_for_strength(50);
    
    REQUIRE(weights[0] == 0);
    REQUIRE(weights[1] == 481);
    REQUIRE(weights[2] == 845);
    
  }
}
