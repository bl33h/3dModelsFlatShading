/*---------------------------------------------------------------------------
Copyright (C), 2022-2023, Sara Echeverria (bl33h)
@author Sara Echeverria
FileName: trianglefill.h
@version: I
Creation: 22/08/2023
Last modification: 15/09/2023
*Some parts were made using the AIs Bard and ChatGPT
------------------------------------------------------------------------------*/

#pragma once
#include <vector>
#include <glm/glm.hpp>
#include "fragment.h"
#include "vertex.h"
#include "framebuffer.h"

extern glm::vec3 L;

std::vector<Fragment> triangleFill(const Vertex& a, const Vertex& b, const Vertex& c);
