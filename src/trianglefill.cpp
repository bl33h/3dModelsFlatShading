/*---------------------------------------------------------------------------
Copyright (C), 2022-2023, Sara Echeverria (bl33h)
@author Sara Echeverria
FileName: trianglefill.cpp
@version: I
Creation: 22/08/2023
Last modification: 15/09/2023
*Some parts were made using the AIs Bard and ChatGPT
------------------------------------------------------------------------------*/

#include "trianglefill.h"

glm::vec3 barycentricCoordinates(const glm::ivec2& P, const glm::vec3& A, const glm::vec3& B, const glm::vec3& C);

std::vector<Fragment> triangleFill(const Vertex& a, const Vertex& b, const Vertex& c) {
    std::vector<Fragment> fragments;

    // Encontrar las coordenadas barycentricas del triángulo
    glm::vec3 A = a.position;
    glm::vec3 B = b.position;
    glm::vec3 C = c.position;
    
    float minX = std::min(std::min(A.x, B.x), C.x);
    float minY = std::min(std::min(A.y, B.y), C.y);
    float maxX = std::max(std::max(A.x, B.x), C.x);
    float maxY = std::max(std::max(A.y, B.y), C.y);

    for (int y = static_cast<int>(std::ceil(minY)); y <= static_cast<int>(std::floor(maxY)); ++y) {
        for (int x = static_cast<int>(std::ceil(minX)); x <= static_cast<int>(std::floor(maxX)); ++x) {
            if (x < 0 || y < 0 || y >= SCREEN_HEIGHT || x >= SCREEN_WIDTH)
                continue;

            glm::ivec2 P(x, y);
            glm::vec3 barycentric = barycentricCoordinates(P, A, B, C);
            float w = barycentric.x;
            float v = barycentric.y;
            float u = barycentric.z;

            if (u >= 0 && v >= 0 && w >= 0) {
                // El punto P está dentro del triángulo
                double z = a.position.z * w + b.position.z * v + c.position.z * u;

                // Interpolar atributos (en este caso, colores)
                Color interpolatedColor = a.color * w + b.color * v + c.color * u;
                float intensity = a.intensity * w + b.intensity * v + c.intensity * u;

                if (intensity > 0) {
                    // Asegúrate de que la intensidad sea positiva
                    Fragment fragment{
                        static_cast<uint16_t>(P.x),
                        static_cast<uint16_t>(P.y),
                        z,
                        interpolatedColor,
                        intensity
                    };
                    fragments.push_back(fragment);
                }
            }
        }
    }

    return fragments;
}
