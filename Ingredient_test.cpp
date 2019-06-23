#define CATCH_CONFIG_MAIN
#include "catch.hpp"

// include twice to test for header guard
#include "Ingredient.h"
#include "Ingredient.h"

TEST_CASE("Ingredients test", "[Ingredient]")
{
  Ingredient yum(80, 11340, 97, "vodka", 1);
  
  
  SECTION("save")
  {
    uint8_t p_data[16];
    
    yum.get_data(p_data);
    
    Ingredient yum2(p_data);
    
    REQUIRE(yum.m_alc_content == yum2.m_alc_content);
    REQUIRE(yum.m_est_remaining == yum2.m_est_remaining);
    REQUIRE(yum.m_density == yum2.m_density);
    REQUIRE(strcmp(yum.name, yum2.name) == 0);
    REQUIRE(yum.m_bay == yum2.m_bay);
  }
  
  SECTION("calculate weight")
  {
    REQUIRE(yum.get_weight(100) == 97);
    REQUIRE(yum.get_weight(1000) == 970);
  }
  
  SECTION("calculate alc mass")
  {
    REQUIRE(yum.get_alc_from_weight(1000) == 352);
    REQUIRE(yum.get_alc_from_weight(286) == 101);
  }
  
  SECTION("extreme values")
  {
    Ingredient extreme(0, 0, 0, "", 0);
    
    REQUIRE(extreme.get_weight(100) == 0);
    REQUIRE(extreme.get_alc_from_weight(100) == 0);
  }
  
}
