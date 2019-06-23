#ifndef DRINK_MAKER_RECIPE_H
#define DRINK_MAKER_RECIPE_H

#include <stdint.h>

// forward declaration
class Cookbook;

class Recipe
{
public:
  Recipe(Cookbook* cookbook, const char* name, uint8_t num_ingredients, uint8_t num_points, uint8_t** p_data);
  
  //  Recipe(Cookbook* cookbook, uint8_t* p_data));
  
  // get the default strenght in percent
  uint8_t get_default_strength();
  
  // strength in percent
  // weights are in g * 10
  uint16_t* get_weights_for_strength(uint8_t strength);
  
  // updated for when an ingredient is added
  void update_added_ingredient();
  
  // add a new point to the recipe
  void add_new_point(const uint8_t* p_point);
  
  // gets the size of the saved object
  uint8_t size();
  
  // get the data to save the object
//  void get_data(uint8_t* data);
  
private:
  
  void calculate_strengths();
  
  Cookbook* m_p_cookbook;
  
  char* m_p_name;
  
  uint8_t m_num_ingredients;
  uint8_t m_num_points;
  uint8_t** m_p_data;
  
  uint8_t* m_p_strengths;
};

#endif
