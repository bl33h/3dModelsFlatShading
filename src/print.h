/*---------------------------------------------------------------------------
Copyright (C), 2022-2023, Sara Echeverria (bl33h)
@author Sara Echeverria
FileName: print.h
@version: I
Creation: 22/08/2023
Last modification: 15/09/2023
*Some parts were made using the AIs Bard and ChatGPT
------------------------------------------------------------------------------*/
#pragma once

#include <iostream>
#include <glm/glm.hpp>
#include "colors.h"
#include "triangles.h"
#include "fragment.h"

// Function to print an empty line
inline void print() 
{
    std::cout << std::endl;
}

// Function to print a Vertex structure
void print(const Vertex& v) 
{
    std::cout << "Vertex{";
    std::cout << "(" << v.position.x << ", " << v.position.y << ", " << v.position.z << ")";
    std::cout << "}" << std::endl;
}

// Function to print a Color structure
void print(const Color& c) 
{
    std::cout << "Color("
        << static_cast<int>(c.r) 
        << ", "
        << static_cast<int>(c.g) 
        << ", "
        << static_cast<int>(c.b) 
        << ")"
    << std::endl;
}

// Function to print a glm::ivec2 structure
void print(const glm::ivec2& v) 
{
    std::cout << "glm::vec2(" << v.x << ", " << v.y << ")" << std::endl;
}

// Function to print a glm::vec3 structure
void print(const glm::vec3& v) 
{
    std::cout << "glm::vec3(" << v.x << ", " << v.y << ", " << v.z << ")" << std::endl;
}

// Function to print a glm::vec4 structure
void print(const glm::vec4& v) 
{
    std::cout << "glm::vec4(" << v.x << ", " << v.y << ", " << v.z << ", " << v.w << ")" << std::endl;
}

// Function to print a glm::mat4 structure
void print(const glm::mat4& m) 
{
    std::cout << "glm::mat4(\n";
    for(int i = 0; i < 4; ++i) {
        std::cout << "  ";
        for(int j = 0; j < 4; ++j) 
        {
            std::cout << m[i][j];
            if (j != 3) {
                std::cout << ", ";
            }
        }
        std::cout << (i == 3 ? "\n" : ",\n");
    }
    std::cout << ")" << std::endl;
}

// Template function to print a variable, with an optional boolean flag to indicate if it's the last item in the output
template <typename T>
inline void print(const T& one, bool last = true) 
{
    std::cout << one;
    if (last) 
    {
        std::cout << std::endl;
    }
}

// Variadic template function to print multiple variables, separated by spaces
template <typename T, typename... Args>
inline void print(const T& first, const Args&... args) 
{
    print(first, false); 

    if constexpr (sizeof...(args) > 0) 
    {
        std::cout << ' '; 
        print(args...);
    } else {
        std::cout << std::endl;
    }
}