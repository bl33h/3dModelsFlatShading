/*---------------------------------------------------------------------------
Copyright (C), 2022-2023, Sara Echeverria (bl33h)
@author Sara Echeverria
FileName: shaders.h
@version: I
Creation: 22/08/2023
Last modification: 15/09/2023
*Some parts were made using the AIs Bard and ChatGPT
------------------------------------------------------------------------------*/
#pragma once
#include <glm/glm.hpp>
#include "fragment.h"
#include "print.h"

// Fragment shader that calculates the final color of a fragment based on its intensity
Fragment fragmentShader(Fragment& fragment) 
{
    // Scale the color of the fragment by its intensity
    fragment.color = fragment.color * fragment.intensity;

    return fragment;
}

// Vertex shader that transforms a vertex's position and normal using model-view-projection matrices
Vertex vertexShader(const Vertex& vertex, const ShaderData& ShaderData) 
{
    // Transform the vertex's position using model-view-projection matrices
    glm::vec4 clipSpaceVertex = ShaderData.projection * ShaderData.view * ShaderData.model * glm::vec4(vertex.position, 1.0f);
    glm::vec3 ndcVertex = glm::vec3(clipSpaceVertex) / clipSpaceVertex.w;
    glm::vec4 screenVertex = ShaderData.viewport * glm::vec4(ndcVertex, 1.0f);

    // Transform the vertex's normal using the model matrix
    glm::vec3 transformedNormal = glm::mat3(ShaderData.model) * vertex.normal;
    transformedNormal = glm::normalize(transformedNormal);
    
    return Vertex{
        glm::vec3(screenVertex),
        transformedNormal,
    };
}