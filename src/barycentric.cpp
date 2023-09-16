/*---------------------------------------------------------------------------
Copyright (C), 2022-2023, Sara Echeverria (bl33h)
@author Sara Echeverria
FileName: barycentric.cpp
@version: I
Creation: 22/08/2023
Last modification: 15/09/2023
*Some parts were made using the AIs Bard and ChatGPT
------------------------------------------------------------------------------*/

#include "triangles.h"

// Define a light vector L for shading
glm::vec3 L = glm::vec3(0.0f, 0.0f, 1.0f);

// Function to calculate barycentric coordinates of a point in a triangle
std::pair<float, float> barycentricCoordinates(const glm::ivec2& P, const glm::vec3& A, const glm::vec3& B, const glm::vec3& C) 
{
    // Calculate barycentric coordinates using cross products
    glm::vec3 bary = glm::cross(
        glm::vec3(C.x - A.x, B.x - A.x, A.x - P.x),
        glm::vec3(C.y - A.y, B.y - A.y, A.y - P.y)
    );

    // Check if the point is inside the triangle (abs(z) close to zero)
    if (abs(bary.z) < 1) 
    {
        return std::make_pair(-1, -1); // Return invalid coordinates
    }

    // Calculate and return valid barycentric coordinates
    return std::make_pair
    (
        bary.y / bary.z,
        bary.x / bary.z
    );
}

// Function to rasterize a triangle and generate fragments
std::vector<Fragment> triangle(const Vertex& a, const Vertex& b, const Vertex& c) 
{
    std::vector<Fragment> fragments;
    glm::vec3 A = a.position;
    glm::vec3 B = b.position;
    glm::vec3 C = c.position;

    // Determine the bounding box of the triangle
    float minX = std::min(std::min(A.x, B.x), C.x);
    float minY = std::min(std::min(A.y, B.y), C.y);
    float maxX = std::max(std::max(A.x, B.x), C.x);
    float maxY = std::max(std::max(A.y, B.y), C.y);

    // Loop through the bounding box and rasterize the triangle
    for (int y = static_cast<int>(std::ceil(minY)); y <= static_cast<int>(std::floor(maxY)); ++y) 
    {
        for (int x = static_cast<int>(std::ceil(minX)); x <= static_cast<int>(std::floor(maxX)); ++x) 
        {
            if (x < 0 || y < 0 || y > windowHeight || x > windowWith)
                continue;

            // Create a point P at (x, y)
            glm::ivec2 P(x, y);

            // Calculate barycentric coordinates for P
            auto barycentric = barycentricCoordinates(P, A, B, C);

            // Extract barycentric coordinates
            float w = 1 - barycentric.first - barycentric.second;
            float v = barycentric.first;
            float u = barycentric.second;

            float epsilon = 1e-10;

            // Check if P is inside the triangle using barycentric coordinates
            if (w < epsilon || v < epsilon || u < epsilon)
                continue;

            // Interpolate the depth (z) and normal for P
            double z = A.z * w + B.z * v + C.z * u;
            glm::vec3 normal = glm::normalize
            (
                a.normal * w + b.normal * v + c.normal * u
            ); 

            // Calculate the intensity of the light on P
            float intensity = glm::dot(normal, L);

            // Discard fragments with negative intensity
            if (intensity < 0)
                continue;

            // Create a fragment and add it to the list
            fragments.push_back
            (
                Fragment{
                    static_cast<uint16_t>(P.x),
                    static_cast<uint16_t>(P.y),
                    z,
                    Color(205, 205, 205),
                    intensity}
            );
        }
    }

    return fragments; // Return the list of fragments generated
}