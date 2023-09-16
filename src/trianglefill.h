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
#include <glm/glm.hpp>
#include <array>
#include <vector>

// Structure representing a face in the OBJ file
struct Face
{
    std::array<int, 3> vertexIndices;  // Indices of vertices for the face
    std::array<int, 3> normalIndices;  // Indices of normals for the face
};

// Function to parse an OBJ file and extract its vertices, normals, and faces
bool triangleFiller
(
    const std::string& path,
    std::vector<glm::vec3>& out_vertices,
    std::vector<glm::vec3>& out_normals,
    std::vector<Face>& out_faces
);