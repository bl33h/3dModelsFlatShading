/*---------------------------------------------------------------------------
Copyright (C), 2022-2023, Sara Echeverria (bl33h)
@author Sara Echeverria
FileName: border.cpp
@version: I
Creation: 22/08/2023
Last modification: 15/09/2023
*Some parts were made using the AIs Bard and ChatGPT
------------------------------------------------------------------------------*/

#include "fragment.h"

// Function to rasterize a line between two 3D points
std::vector<Fragment> line(const glm::vec3& v1, const glm::vec3& v2) 
{
    // Convert 3D points to 2D integer coordinates
    glm::ivec2 p1(static_cast<int>(v1.x), static_cast<int>(v1.y));
    glm::ivec2 p2(static_cast<int>(v2.x), static_cast<int>(v2.y));

    std::vector<Fragment> fragments;
    int dx = std::abs(p2.x - p1.x);
    int dy = std::abs(p2.y - p1.y);
    int sx = (p1.x < p2.x) ? 1 : -1;
    int sy = (p1.y < p2.y) ? 1 : -1;
    int err = dx - dy;
    glm::ivec2 current = p1;

    while (true) 
    {
        // Create a fragment for the current pixel
        Fragment fragment;
        fragment.x = current.x;
        fragment.y = current.y;
        fragments.push_back(fragment);

        // Check if we've reached the end point
        if (current == p2) 
        {
            break;
        }

        // Calculate the error term for pixel placement
        int e2 = 2 * err;

        // Update the current pixel based on the error term
        if (e2 > -dy) 
        {
            err -= dy;
            current.x += sx;
        }
        if (e2 < dx) 
        {
            err += dx;
            current.y += sy;
        }
    }

    return fragments; // Return the list of fragments representing the line
}