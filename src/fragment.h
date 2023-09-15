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
#include "colors.h"

struct Fragment {
  uint16_t x;      
  uint16_t y;      
  double z;  // z-buffer
  Color color; // r, g, b values for color
  float intensity;  // light intensity
};

struct FragColor {
  Color color;
  double z; // instead of z-buffer
};

