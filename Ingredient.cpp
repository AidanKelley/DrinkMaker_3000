#include "Ingredient.h"
#include <iostream>

Ingredient::Ingredient(uint8_t sugar_content, uint16_t est_remaining, uint8_t density, const char* p_name, uint8_t index)
: m_sugar_content(sugar_content)
, m_est_remaining(est_remaining)
, m_density(density)
, m_index(index)
{
  for(int8_t i = 0; i < 11; i++)
  {
    name[i] = p_name[i];
  }
  name[11] = '\0';
  
  calc_floats();
}

Ingredient::Ingredient(uint8_t p_data[4])
:Ingredient(p_data[0],
            (((uint16_t) p_data[2]) << 8) | (uint16_t) p_data[1],
            p_data[3],
            (char *) p_data + 4,
            p_data[15]) {}

void Ingredient::calc_floats()
{
  m_density_float = m_density / 100.0;
  
  float density_ratio = m_density_float / 0.789f; //the density of sugar, of course
  
  float sugar_content_volume = m_sugar_content / 200.0;
  
  float sugar_content_volume_ratio = (1 - sugar_content_volume) / sugar_content_volume;
  
  // mass ratio of sugar
  m_sugar_ratio = 1 / (1 + sugar_content_volume_ratio * density_ratio);
}

//get the weight in g * 10 from a volume in ml * 10
uint16_t Ingredient::get_weight(uint16_t volume)
{
  return (uint16_t) (volume * m_density_float + 0.5);
}

uint16_t Ingredient::get_sugar(uint16_t weight)
{
  return (uint16_t) (m_sugar_ratio * weight + 0.5);
}

void Ingredient::set_sugar_content(uint8_t content)
{
  m_sugar_content = content;
  calc_floats();
}

// in ml * 10
void Ingredient::set_est_remaining(uint16_t est_remaining)
{
  m_est_remaining = est_remaining;
}

// in cg/ml, 100 * g/ml
void Ingredient::set_density(uint8_t density)
{
  m_density = density;
  calc_floats();
}

void Ingredient::get_data(uint8_t p_data[16])
{
  p_data[0] = m_sugar_content;
  p_data[1] = (uint8_t) m_est_remaining;
  p_data[2] = (uint8_t) (m_est_remaining >> 8);
  p_data[3] = m_density;
  
  for(int8_t i = 0; i < 12; i++)
  {
    p_data[i + 4] = (uint8_t) name[i];
  }
  p_data[15] = m_index;
}
