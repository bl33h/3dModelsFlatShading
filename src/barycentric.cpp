/*---------------------------------------------------------------------------
Copyright (C), 2022-2023, Sara Echeverria (bl33h)
@author Sara Echeverria
FileName: barycentric.cpp
@version: I
Creation: 22/08/2023
Last modification: 15/09/2023
*Some parts were made using the AIs Bard and ChatGPT
------------------------------------------------------------------------------*/

glm::vec3 barycentricCoordinates(const glm::ivec2& P, const glm::vec3& A, const glm::vec3& B, const glm::vec3& C) {

    glm::vec2 v0 = glm::vec2(A.x, A.y);
    glm::vec2 v1 = glm::vec2(B.x, B.y);
    glm::vec2 v2 = glm::vec2(C.x, C.y);

    glm::vec2 v01 = v1 - v0;
    glm::vec2 v02 = v2 - v0;
    glm::vec2 vp = glm::vec2(P.x, P.y) - v0;

    float denominator = v01.x * v02.y - v01.y * v02.x;

    float u = (v02.y * vp.x - v02.x * vp.y) / denominator;
    float v = (-v01.y * vp.x + v01.x * vp.y) / denominator;
    float w = 1.0f - u - v;

    return glm::vec3(u, v, w);
}
