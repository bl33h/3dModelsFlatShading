/*---------------------------------------------------------------------------
Copyright (C), 2022-2023, Sara Echeverria (bl33h)
@author Sara Echeverria
FileName: main.cpp
@version: I
Creation: 12/08/2023
Last modification: 15/09/2023
*Some parts were made using the AIs Bard and ChatGPT
------------------------------------------------------------------------------*/

#include "colors.h"
#include "faces.h"
#include "triangles.h"
#include "trianglefill.cpp"
#include "barycentric.cpp"
#include <iostream>
#include <SDL2/SDL.h>
#include <vector>
#include <array>
#include <fstream>
#include <sstream>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

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

// Function to draw triangles using lines between three vertices
void trianglesDrawing(const glm::vec3& v1, const glm::vec3& v2, const glm::vec3& v3) 
{
    linesDrawing(v1, v2);
    linesDrawing(v2, v3);
    linesDrawing(v3, v1);
}

// Function to draw the model using lines, vertices, and triangles
void drawModel(const std::vector<glm::vec3>& vertex, const std::vector<Face>& faces, const glm::vec3& lightDirection) {
    // Clear the renderer with the background color
    SDL_SetRenderDrawColor(renderer, clearColor.r, clearColor.g, clearColor.b, clearColor.a);
    SDL_RenderClear(renderer);

    // Set the renderer color for drawing lines
    SDL_SetRenderDrawColor(renderer, currentColor.r, currentColor.g, currentColor.b, currentColor.a);

    // Calculate normals for vertices
    std::vector<glm::vec3> normals;
    calculateNormals(vertex, faces, normals);

    // Iterate through faces and draw filled triangles
    for (const auto& face : faces) {
        if (face.vertexIndices.size() == 3) {
            glm::vec3 v1 = vertex[face.vertexIndices[0]];
            glm::vec3 v2 = vertex[face.vertexIndices[1]];
            glm::vec3 v3 = vertex[face.vertexIndices[2]];

            // Calculate normal for this face (average of vertex normals)
            glm::vec3 faceNormal = (normals[face.vertexIndices[0]] + normals[face.vertexIndices[1]] + normals[face.vertexIndices[2]]) / 3.0f;

            // Calculate light intensity for this face
            float intensity = calculateLightIntensity(faceNormal, lightDirection);

            // Convert intensity to grayscale color
            unsigned char gray = static_cast<unsigned char>(intensity * 255.0f);
            Color grayColor(gray, gray, gray, 255);

            int offsetX = windowWidth / 2;
            int offsetY = windowHeight / 1.5;

            // Fill the triangle with the calculated color
            Vertex vertex1;
            vertex1.position = v1 + glm::vec3(offsetX, offsetY, 0);
            vertex1.color = grayColor;
            vertex1.intensity = intensity;

            Vertex vertex2;
            vertex2.position = v2 + glm::vec3(offsetX, offsetY, 0);
            vertex2.color = grayColor;
            vertex2.intensity = intensity;

            Vertex vertex3;
            vertex3.position = v3 + glm::vec3(offsetX, offsetY, 0);
            vertex3.color = grayColor;
            vertex3.intensity = intensity;

            std::vector<Fragment> fragments = triangleFill(vertex1, vertex2, vertex3);

            // Draw the fragments
            for (const auto& fragment : fragments) {
                SDL_SetRenderDrawColor(renderer, fragment.color.r, fragment.color.g, fragment.color.b, fragment.color.a);
                SDL_RenderDrawPoint(renderer, fragment.x, fragment.y);
            }
        }
    }

    // Present the renderer
    SDL_RenderPresent(renderer);
}

// Function to load a 3D object from an OBJ file
bool load3Dobject(const std::string& path, std::vector<glm::vec3>& outputVertex, std::vector<Face>& out_faces) 
{   
    // Open the OBJ file
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cout << "Error: Couldn't open file " << path << std::endl;
        return false;
    }

    // Temporary storage for vertices and faces
    std::vector<glm::vec3> temporaryVertex;
    std::vector<std::array<int, 3>> temp_faces;

    // Read through the file line by line
    std::string line;
    while (std::getline(file, line)) 
    {
        std::istringstream iss(line);
        std::string type;
        iss >> type;

        // Parse vertex data
        if (type == "v") {
            glm::vec3 vertex;
            iss >> vertex.x >> vertex.y >> vertex.z;
            temporaryVertex.push_back(vertex);
        
        // Parse face data
        } else if (type == "f") {
            std::array<int, 3> faceID{};
            for (int i = 0; i < 3; i++) {
                std::string faceIndexStr;
                iss >> faceIndexStr;
                faceID[i] = std::stoi(faceIndexStr) - 1;
            }
            temp_faces.push_back(faceID);
        }
    }

    // Store the parsed data into output vectors
    outputVertex = temporaryVertex;
    out_faces.reserve(temp_faces.size());
    for (const auto& face : temp_faces) {
        out_faces.push_back({ face });
    }

    return true;
}

// Function to set up the vertex array for rendering
std::vector<glm::vec3> setupVertexArray(const std::vector<glm::vec3>& vertex, const std::vector<Face>& faces)
{    
    // Scale factor for vertices
    float scale = 40.0f;

    // Initialize vertex array
    std::vector<glm::vec3> vertexArray;

    // Process object's faces
    for (const auto& face : faces)
    {   
        // Get vertex positions and scale
        glm::vec3 vertexPosition1 = vertex[face.vertexIndices[0]];
        glm::vec3 vertexPosition2 = vertex[face.vertexIndices[1]];
        glm::vec3 vertexPosition3 = vertex[face.vertexIndices[2]];
        glm::vec3 vertexScaled1 = glm::vec3(vertexPosition1.x * scale, -vertexPosition1.y * scale, vertexPosition1.z * scale);
        glm::vec3 vertexScaled2 = glm::vec3(vertexPosition2.x * scale, -vertexPosition2.y * scale, vertexPosition2.z * scale);
        glm::vec3 vertexScaled3 = glm::vec3(vertexPosition3.x * scale, -vertexPosition3.y * scale, vertexPosition3.z * scale);

        // Add scaled vertices to the vertex array
        vertexArray.push_back(vertexScaled1);
        vertexArray.push_back(vertexScaled2);
        vertexArray.push_back(vertexScaled3);
    }

    return vertexArray;
}

int main() {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0) 
    {
        std::cout << "Error: SDL_Init failed." << std::endl;
        return 1;
    }

    // Create SDL window
    SDL_Window* window = SDL_CreateWindow("3dModelLoader", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowWidth, windowHeight, SDL_WINDOW_SHOWN);
    if (!window) 
    {
        std::cout << "Error: Could not create SDL window." << std::endl;
        SDL_Quit();
        return 1;
    }

    // Create SDL renderer
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) 
    {
        std::cout << "Error: Could not create SDL renderer." << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Load 3D model from OBJ file (here you can change the path in case you did not place the file in the objects folder)
    std::string filePath = "src/objects/spaceship.obj";
    std::cout << "Loading OBJ file: " << filePath << std::endl;

    // Initialize vectors to store vertex and face data
    std::vector<glm::vec3> vertex;
    std::vector<Face> faces;

    // Load the 3D model from the OBJ file
    if (!load3Dobject(filePath, vertex, faces)) 
    {
        std::cout << "Error: Could not load OBJ file." << std::endl;
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Set up the vertex array for rendering
    std::vector<glm::vec3> vertexArray = setupVertexArray(vertex, faces);

    // Clear the renderer and set drawing color
    SDL_SetRenderDrawColor(renderer, clearColor.r, clearColor.g, clearColor.b, clearColor.a);
    SDL_RenderClear(renderer);

    // Define la dirección de la fuente de luz (ajústala según tus necesidades)
    glm::vec3 lightDirection(0.0f, -1.0f, 0.0f);

    // Draw the initial model
    SDL_SetRenderDrawColor(renderer, currentColor.r, currentColor.g, currentColor.b, currentColor.a);

    // Llama a drawModel con los argumentos requeridos
    drawModel(vertexArray, faces, lightDirection);

    SDL_RenderPresent(renderer);

    // Main loop for rendering and interaction
    bool running = true;
    SDL_Event event;
    float angle = 0.0f;

    while (running) {

        // Check for events
        while (SDL_PollEvent(&event)) 
        {
            // Exit the loop if the window is closed
            if (event.type == SDL_QUIT) 
            {
                running = false;
            }
        }

        // Clear the renderer
        SDL_SetRenderDrawColor(renderer, clearColor.r, clearColor.g, clearColor.b, clearColor.a);
        SDL_RenderClear(renderer);

        // Update rotation angle
        angle += 0.01f;

        // Create a rotation matrix around the Y-axis
        glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0f, 1.0f, 0.0f));

        // Create a rotation matrix around the X-axis (vertical rotation)
        glm::mat4 rotationMatrixVertical = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(1.0f, 0.0f, 0.0f));

        // Combine the rotation matrices for horizontal and vertical rotations
        glm::mat4 combinedRotationMatrix = rotationMatrixVertical * rotationMatrix;

        // Apply combined rotation to each vertex in the vertex array
        std::vector<glm::vec3> rotatedVertexArray;
        for (const auto& vertex : vertexArray) 
        {
            glm::vec4 rotatedVertex = combinedRotationMatrix * glm::vec4(vertex, 1.0f);
            rotatedVertexArray.push_back(glm::vec3(rotatedVertex));
        }

        // Draw the rotated model
        drawModel(rotatedVertexArray, faces, lightDirection);
        SDL_RenderPresent(renderer);
    }

    // Clean up and exit
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}