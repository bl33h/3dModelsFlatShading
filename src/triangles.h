/*---------------------------------------------------------------------------
Copyright (C), 2022-2023, Sara Echeverria (bl33h)
@author Sara Echeverria
FileName: triangles.h
@version: I
Creation: 10/08/2023
Last modification: 10/08/2023
*Some parts were made using the AIs Bard and ChatGPT
------------------------------------------------------------------------------*/

#pragma once
#include <glm/glm.hpp>
#include <vector>
#include "colors.h"
#include "border.h"
#include "framebuffer.h"

// Structure representing a vertex with position and normal
struct Vertex 
{
    glm::vec3 position;  // Vertex position
    glm::vec3 normal;    // Vertex normal
};

// External variable representing the light direction
extern glm::vec3 L;

// Function to calculate barycentric coordinates for a point within a triangle
std::pair<float, float> barycentricCoordinates(const glm::ivec2& P, const glm::vec3& A, const glm::vec3& B, const glm::vec3& C);

// Function to rasterize a triangle and return its fragments
std::vector<Fragment> triangle(const Vertex& a, const Vertex& b, const Vertex& c);