/*---------------------------------------------------------------------------
Copyright (C), 2022-2023, Sara Echeverria (bl33h)
@author Sara Echeverria
FileName: main.cpp
@version: I
Creation: 12/08/2023
Last modification: 16/09/2023
*Some parts were made using the AIs Bard and ChatGPT
------------------------------------------------------------------------------*/

#include <SDL2/SDL.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <sstream>
#include <vector>
#include <cassert>
#include "barycentric.cpp"
#include "framebuffer.cpp"
#include "border.cpp"
#include "trianglefill.cpp"
#include "colors.h"
#include "framebuffer.h"
#include "fragment.h"
#include "border.h"
#include "print.h"
#include "shaders.h"
#include "trianglefill.h"
#include "triangles.h"

// Define a global variable for the current color
Color currentColor;

// Function to set the current color
void setColor(const Color& color) 
{
    currentColor = color;
}

// Function to transform each vertex using a shader
std::vector<Vertex> shadingEachVertex(const std::vector<glm::vec3>& VBO, const ShaderData& ShaderData) 
{
    // Create a vector to store the transformed vertices
    std::vector<Vertex> transformedVertex(VBO.size() / 2);

    // Loop through the input vertices and apply the vertex shader
    for (size_t i = 0; i < VBO.size() / 2; ++i) {
        Vertex vertex = { VBO[i * 2], VBO[i * 2 + 1] };
        transformedVertex[i] = vertexShader(vertex, ShaderData);
    }

    return transformedVertex;
}

// Function to gather initial vertices into triangles
std::vector<std::vector<Vertex>> initialGathering(const std::vector<Vertex>& transformedVertex) 
{
    // Create a vector to store groups of vertices representing triangles
    std::vector<std::vector<Vertex>> placedVertex(transformedVertex.size() / 3);

    // Loop through the transformed vertices and group them into triangles
    for (size_t i = 0; i < transformedVertex.size() / 3; ++i) 
    {
        Vertex edge1 = transformedVertex[3 * i];
        Vertex edge2 = transformedVertex[3 * i + 1];
        Vertex edge3 = transformedVertex[3 * i + 2];
        placedVertex[i] = { edge1, edge2, edge3 };
    }

    return placedVertex;
}

// Function to convert vertices into fragments
std::vector<Fragment> pixelsConvertion(const std::vector<std::vector<Vertex>>& placedVertex) 
{
    // Create a vector to store fragments
    std::vector<Fragment> parallelFragments;

    // Loop through the groups of vertices and convert them to fragments
    for (size_t i = 0; i < placedVertex.size(); ++i) 
    {
        std::vector<Fragment> pixeledTriangle = triangle(
            placedVertex[i][0],
            placedVertex[i][1],
            placedVertex[i][2]
        );

        // Ensure thread safety while adding fragments
        std::lock_guard<std::mutex> lock(std::mutex);

        // Add the fragments to the vector
        for (const auto& fragment : pixeledTriangle) 
        {
            parallelFragments.push_back(fragment);
        }
    }

    return parallelFragments;
}

// Function to initialize SDL and create a window
bool init() 
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cout << "!error: init failed, please try again" << std::endl;
        return false;
    }

    // Create an SDL window
    window = SDL_CreateWindow("3D Model's Flat Shading", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowWith, windowHeight, SDL_WINDOW_SHOWN);
    if (!window) 
    {
        std::cout << "!error: couldn't create SDL window" << std::endl;
        SDL_Quit();
        return false;
    }

    // Create an SDL renderer
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) 
    {
        std::cout << "!error: couldn't create SDL renderer" << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return false;
    }

    return true;
}

// Function to apply shading to each fragment
void shadingEachFragment( std::vector<Fragment>& parallelFragments) 
{
    for (size_t i = 0; i < parallelFragments.size(); ++i) {
        // Apply fragment shader to each fragment
        const Fragment& fragment = fragmentShader(parallelFragments[i]);
        point(fragment);
    }
}

// Function to render the scene
void render(const std::vector<glm::vec3>& VBO, const ShaderData& ShaderData) 
{
    // Transform vertices, group them into triangles, and convert to fragments
    std::vector<Vertex> transformedVertex = shadingEachVertex(VBO, ShaderData);
    std::vector<std::vector<Vertex>> placedVertex = initialGathering(transformedVertex);
    std::vector<Fragment> parallelFragments = pixelsConvertion(placedVertex);

    // Apply shading to each fragment
    shadingEachFragment(parallelFragments);
}

// Function to create a viewport matrix
glm::mat4 createViewportMatrix(size_t screenWidth, size_t screenHeight) 
{
    glm::mat4 viewport = glm::mat4(1.0f);
    viewport = glm::scale(viewport, glm::vec3(screenWidth / 2.0f, screenHeight / 2.0f, 0.5f));
    viewport = glm::translate(viewport, glm::vec3(1.0f, 1.0f, 0.5f));
    return viewport;
}

int main(int argc, char* argv[]) 
{
    // Initialize SDL and create a window
    if (!init()) 
    {
        return 1;
    }

    // Load 3D model data
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    std::vector<Face> faces;
    std::vector<glm::vec3> vertexBufferObject;
    std::string filePath = "src/objects/spaceship.obj"; // 3d model file path
    float angle = 0.0f;
    float verticalAngle = 0.0f;

    // Load 3D model from file
    if (!triangleFiller(filePath, vertices, normals, faces)) 
    {
        std::cout << "Error: Could not load OBJ file." << std::endl;

        // Clean up SDL resources
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Prepare vertex buffer object for rendering
    for (const auto& face : faces)
    {
        for (int i = 0; i < 3; ++i)
        {
            glm::vec3 vertexPosition = vertices[face.vertexIndices[i]];
            glm::vec3 vertexNormal = normals[face.normalIndices[i]];
            vertexBufferObject.push_back(vertexPosition);
            vertexBufferObject.push_back(vertexNormal);
        }
    }

    // Initialize shader data and transformation matrices
    ShaderData ShaderData;
    glm::vec3 translationVector(0.0f, 0.0f, 0.0f);
    glm::vec3 scaleFactor(1.0f, 1.0f, 1.0f);
    glm::mat4 translation = glm::translate(glm::mat4(1.0f), translationVector);
    glm::mat4 scale = glm::scale(glm::mat4(1.0f), scaleFactor);

    // Initialize Point of View (POV) parameters
    Pov Pov;
    Pov.PovPosition = glm::vec3(0.0f, 0.0f, 20.0f);
    Pov.targetPosition = glm::vec3(0.0f, 0.0f, 0.0f);
    Pov.upVector = glm::vec3(0.0f, 1.0f, 0.0f);

    // Set up projection matrix
    float fovRadians = glm::radians(45.0f);
    float aspectRatio = static_cast<float>(windowWith) / static_cast<float>(windowHeight);
    float nearClip = 0.1f;
    float farClip = 100.0f;
    ShaderData.projection = glm::perspective(fovRadians, aspectRatio, nearClip, farClip);
    ShaderData.viewport = createViewportMatrix(windowWith, windowHeight);

    bool running = true;
    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
        }

        // Update transformation matrices for rendering
        angle += 0.01f;
        verticalAngle += 0.01f;
        glm::mat4 rotationMatrixHorizontal = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0f, 1.0f, 0.0f));
        glm::mat4 rotationMatrixVertical = glm::rotate(glm::mat4(1.0f), verticalAngle, glm::vec3(1.0f, 0.0f, 0.0f));
        glm::mat4 combinedRotationMatrix = rotationMatrixVertical * rotationMatrixHorizontal;
        ShaderData.model = translation * combinedRotationMatrix * scale;
        ShaderData.view = glm::lookAt(
            Pov.PovPosition,
            Pov.targetPosition,
            Pov.upVector     
        );

        // Render the scene
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderClear(renderer);
        clearFramebuffer();
        setColor(Color(255, 255, 255));
        render(vertexBufferObject, ShaderData);
        renderBuffer(renderer);
    }

    // Clean up SDL resources
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}