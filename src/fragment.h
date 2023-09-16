/*---------------------------------------------------------------------------
Copyright (C), 2022-2023, Sara Echeverria (bl33h)
@author Sara Echeverria
FileName: fragment.h
@version: I
Creation: 22/08/2023
Last modification: 15/09/2023
*Some parts were made using the AIs Bard and ChatGPT
------------------------------------------------------------------------------*/

#pragma once
#include <cstdint>
#include <glm/glm.hpp>
#include "colors.h"

// Structure representing the Point of View (POV)
struct Pov 
{
  glm::vec3 PovPosition;  // Position of the point of view
  glm::vec3 targetPosition;  // Position of the target or focus point
  glm::vec3 upVector;  // Upward vector of the POV
};

// Structure representing a Fragment in computer graphics
struct Fragment 
{
  uint16_t x;      // X-coordinate of the fragment
  uint16_t y;      // Y-coordinate of the fragment
  double z;        // Z-coordinate of the fragment
  Color color;     // Color of the fragment
  float intensity; // Intensity of the fragment's color
};

// Structure representing a Fragment's color and depth
struct FragmentColor 
{
  Color color;  // Color of the fragment
  double z;     // Depth or Z-coordinate of the fragment
};

// Structure containing shader-related data for rendering
struct ShaderData 
{
    glm::mat4 model;      
    glm::mat4 view;        
    glm::mat4 projection;  
    glm::mat4 viewport;    
};