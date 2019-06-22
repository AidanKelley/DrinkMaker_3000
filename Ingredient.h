#ifndef DRINK_MAKER_INGREDIENT_H
#define DRINK_MAKER_INGREDIENT_H

#include <stdint.h>

class Ingredient {
public:
  Ingredient(uint8_t sugar_content, uint16_t est_remaining, uint8_t density, const char* name, uint8_t index);

  Ingredient(uint8_t p_data[16]);
  
  //get the weight in g * 10 from a volume in ml * 10
  uint16_t get_weight(uint16_t volume);

  //get sugar in g * 10 from a weight in g * 10
  uint16_t get_sugar(uint16_t weight);
  
  // 0 - 200
  void set_sugar_content(uint8_t content);
  
  // in ml * 10
  void set_est_remaining(uint16_t est_remaining);
  
  // in cg/ml, 100 * g/ml
  void set_density(uint8_t density);

  //gets the data so this can be saved
  void get_data(uint8_t data[16]);
//private:
  uint8_t m_sugar_content; //0-255, 0% to 99.6%
  uint16_t m_est_remaining; //dg (divide by 10 to get grams)
  uint8_t m_density; //density in cg/ml (divide by 100 to get g/ml)
  char name[12];
  uint8_t m_index;

  float m_sugar_ratio;
  float m_density_float;
  
  // shared between the two constructors.
  void calc_floats();
};


#endif
