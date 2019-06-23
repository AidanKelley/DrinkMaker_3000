#include "Recipe.h"

#include "Cookbook.h"
#include "Ingredient.h"

Recipe::Recipe(Cookbook* p_cookbook, const char* p_name, uint8_t num_ingredients, uint8_t num_points, uint8_t** p_data)
: m_p_cookbook(p_cookbook)
, m_p_name(new char[16])
, m_num_ingredients(num_ingredients)
, m_num_points(num_points)
, m_p_data(p_data)
, m_p_strengths(nullptr)
{
  calculate_strengths();
  
  bool reached_end = false;
  for(int i = 0; i < 16; i++)
  {
    if (p_name[i] == 0) {
      reached_end = true;
    }
    
    if (reached_end)
    {
      m_p_name[i] = '\0';
    }
    else
    {
      m_p_name[i] = p_name[i];
    }
  }
}

void Recipe::calculate_strengths()
{
  if(m_p_strengths)
  {
    delete m_p_strengths;
  }
  
  m_p_strengths = new uint8_t[m_num_points];
  
  for(int8_t iter_row = 0; iter_row < m_num_points; iter_row++)
  {
    uint16_t alc_weight = 0; //in g * 10
    
    for(int8_t iter_ingredient = 0; iter_ingredient < m_num_ingredients; iter_ingredient++)
    {
      uint8_t volume_oz = m_p_data[iter_row][iter_ingredient];
      Ingredient* ingredient = m_p_cookbook->ingredients[iter_ingredient];
      
      uint16_t weight_dg = ingredient->get_weight(29.5735 * volume_oz + 0.5);
      alc_weight += ingredient->get_alc_from_weight(weight_dg);
    }
    
    uint16_t strength = 5*alc_weight/7; //same as dividing by 140, which in g * 10 is a standard drink, then multiplying by 100 to get the percentage strenght.
    
    if(strength > 250)
    {
      m_p_strengths[iter_row] = 250;
      float scale_down = 250.0 / strength;
      
      for(int iter_ingredient; iter_ingredient < m_num_ingredients; iter_ingredient++)
      {
        m_p_data[iter_row][iter_ingredient] = scale_down * m_p_data[iter_row][iter_ingredient];
      }
    }
    else {
      m_p_strengths[iter_row] = strength;
    }
  }
  
  // now do a sort by strengths
  // selection sort because it's easy
  
  for(int8_t i = 0; i < m_num_points; i++)
  {
    uint8_t max = 0;
    int8_t max_index = i;
    
    for(int8_t j = i; j < m_num_points; j++)
    {
      uint8_t strength = m_p_strengths[j];
      if(strength > max)
      {
        max = strength;
        max_index = j;
      }
    }
    
    //do the swap
    if(i != max_index)
    {
      uint8_t temp = m_p_strengths[i];
      m_p_strengths[i] = m_p_strengths[max_index];
      m_p_strengths[max_index] = temp;
      
      for(int8_t iter_ingredient = 0; iter_ingredient < m_num_ingredients; iter_ingredient++)
      {
        temp = m_p_data[i][iter_ingredient];
        m_p_data[i][iter_ingredient] = m_p_data[max_index][iter_ingredient];
        m_p_data[max_index][iter_ingredient] = temp;
      }
    }
  }
}

uint8_t Recipe::get_default_strength()
{
  return m_p_strengths[0];
}

uint16_t* Recipe::get_weights_for_strength(uint8_t strength)
{
  float volumes[m_num_ingredients];
  
  // first linearly interpolate the volumes
  if(strength >= m_p_strengths[0])
  {
    for(int i = 0; i < m_num_ingredients; i++)
    {
      volumes[i] = ((float) m_p_data[0][i]) * strength / m_p_strengths[0];
    }
  }
  else if(strength < m_p_strengths[m_num_points - 1])
  {
    for(int i = 0; i < m_num_ingredients; i++)
    {
      volumes[i] = ((float) m_p_data[m_num_points - 1][i]) * strength / m_p_strengths[m_num_points-1];
    }
  }
  else {
    for(int iter_points = 0; iter_points < m_num_points - 1; iter_points++)
    {
      // see if strengths is within the band
      if((m_p_strengths[iter_points + 1] <=  strength)
         && (strength < m_p_strengths[iter_points]))
      {
        float part_in_band = ((float)m_p_strengths[iter_points] - (float)strength) / ((float) m_p_strengths[iter_points] - (float) m_p_strengths[iter_points+1]);
        
        for(int i = 0; i < m_num_ingredients; i++)
        {
          float band_delta = (float) m_p_data[iter_points + 1][i] - (float) m_p_data[iter_points][i];
          
          volumes[i] = (float) m_p_data[iter_points][i] + part_in_band * band_delta;

          // just in case
          if(volumes[i] < 0)
          {
            volumes[i] = 0;
          }
        }
      }
    }
  }
  
  // now that we have the volumes (in fl oz), find the weights
  uint16_t* weights = new uint16_t[m_num_ingredients];
  
  for(int i = 0; i < m_num_ingredients; i++)
  {
    weights[i] = m_p_cookbook->ingredients[i]->get_weight(volumes[i] * 29.5735 + 0.5);
  }
  
  return weights;
}

void Recipe::update_added_ingredient()
{
  uint8_t new_num_ingredients = m_p_cookbook->count_ingredients;
  
  
  if(new_num_ingredients > m_num_ingredients)
  {
    uint8_t** p_new_data = new uint8_t*[m_num_points];
    
    for(uint8_t i = 0; i < m_num_points; i++)
    {
      p_new_data[i] = new uint8_t[new_num_ingredients];
      // copy over old data
      for(uint8_t j = 0; j < m_num_ingredients; j++)
      {
        p_new_data[i][j] = m_p_data[i][j];
      }
    }
    
    delete m_p_data;
    m_p_data = p_new_data;
  }
  
  calculate_strengths();
}

void Recipe::add_new_point(const uint8_t* p_point)
{
  uint8_t** p_new_data = new uint8_t*[m_num_points + 1];
  
  // copy over old data
  for(uint8_t i = 0; i < m_num_points; i++)
  {
    p_new_data[i] = new uint8_t[m_num_ingredients];
    
    for(uint8_t j = 0; j < m_num_ingredients; j++)
    {
      p_new_data[i][j] = m_p_data[i][j];
    }
  }
  delete m_p_data;
  
  // copy over the new data
  for(uint8_t j = 0; j < m_num_ingredients; j++)
  {
    p_new_data[m_num_points] = new uint8_t[m_num_ingredients];
    
    // add it to the end... we'll do the sort after
    p_new_data[m_num_points][j] = p_point[j];
  }
  m_p_data = p_new_data;
  
  // incremenet num points
  m_num_points++;
  
  // sort
  calculate_strengths();
}

uint8_t Recipe::size()
{
  return 1 + 16 + m_num_ingredients * m_num_points;
}
