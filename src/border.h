/*---------------------------------------------------------------------------
Copyright (C), 2022-2023, Sara Echeverria (bl33h)
@author Sara Echeverria
FileName: border.h
@version: I
Creation: 22/08/2023
Last modification: 15/09/2023
*Some parts were made using the AIs Bard and ChatGPT
------------------------------------------------------------------------------*/

#pragma once
#include <glm/glm.hpp>
#include <vector>
#include "fragment.h"

// Function prototype for line rasterization
std::vector<Fragment> line(const glm::vec3& v1, const glm::vec3& v2);