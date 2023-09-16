/*---------------------------------------------------------------------------
Copyright (C), 2022-2023, Sara Echeverria (bl33h)
@author Sara Echeverria
FileName: framebuffer.cpp
@version: I
Creation: 22/08/2023
Last modification: 15/09/2023
*Some parts were made using the AIs Bard and ChatGPT
------------------------------------------------------------------------------*/

#include "framebuffer.h"

SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;

// Function to render a point (fragment) on the framebuffer
void point(Fragment f) 
{
    // Lock the mutex for the specific pixel coordinate
    std::lock_guard<std::mutex> lock(mutexes[f.y * windowWith + f.x]);

    // Check if the fragment's depth is less than the current depth at that pixel
    if (f.z < framebuffer[f.y * windowWith + f.x].z) 
    {
        // Update the framebuffer with the new fragment color and depth
        framebuffer[f.y * windowWith + f.x] = FragmentColor{f.color, f.z};
    }
}

// Background color and depth for the framebuffer
FragmentColor background
{
    Color{111, 99, 143},
    std::numeric_limits<double>::max()
};

// Array representing the framebuffer
std::array<FragmentColor, windowWith * windowHeight> framebuffer;

// Array of mutexes for thread-safe access to pixel coordinates
std::array<std::mutex, windowWith * windowHeight> mutexes;

// Function to clear the framebuffer with the background color and depth
void clearFramebuffer() 
{
    std::fill(framebuffer.begin(), framebuffer.end(), background);
}

// Function to render the framebuffer onto the SDL renderer
void renderBuffer(SDL_Renderer* renderer) 
{
    // Create an SDL texture for rendering the framebuffer
    SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, windowWith, windowHeight);
    void* texturePixels;
    int pitch;
    SDL_LockTexture(texture, NULL, &texturePixels, &pitch);
    Uint32 format = SDL_PIXELFORMAT_ARGB8888;
    SDL_PixelFormat* mappingFormat = SDL_AllocFormat(format);
    Uint32* texturePixels32 = static_cast<Uint32*>(texturePixels);

    // Copy the colors from the framebuffer to the texture
    for (int y = 0; y < windowHeight; y++) {
        for (int x = 0; x < windowWith; x++) {
            int framebufferY = windowHeight - y - 1;
            int index = y * (pitch / sizeof(Uint32)) + x;
            const Color& color = framebuffer[framebufferY * windowWith + x].color;
            texturePixels32[index] = SDL_MapRGBA(mappingFormat, color.r, color.g, color.b, color.a);
        }
    }

    SDL_UnlockTexture(texture);
    SDL_Rect textureRect = {0, 0, windowWith, windowHeight};

    // Copy the texture to the SDL renderer for display
    SDL_RenderCopy(renderer, texture, NULL, &textureRect);
    SDL_DestroyTexture(texture);
    SDL_RenderPresent(renderer);
}