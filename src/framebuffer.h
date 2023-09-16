/*---------------------------------------------------------------------------
Copyright (C), 2022-2023, Sara Echeverria (bl33h)
@author Sara Echeverria
FileName: framebuffer.h
@version: I
Creation: 12/08/2023
Last modification: 12/08/2023
*Some parts were made using the AIs Bard and ChatGPT
------------------------------------------------------------------------------*/
#pragma once

#include <glm/glm.hpp>
#include <algorithm>
#include <array>
#include <mutex>
#include <limits>
#include <SDL2/SDL.h>
#include "colors.h"
#include "fragment.h"
#include "triangles.h"

// Width and height of the rendering window
constexpr size_t windowWith = 1200;
constexpr size_t windowHeight = 900;

// Background color and depth for the framebuffer
extern FragmentColor background;

// Array representing the framebuffer
extern std::array<FragmentColor, windowWith * windowHeight> framebuffer;

// Array of mutexes for thread-safe access to pixel coordinates
extern std::array<std::mutex, windowWith * windowHeight> mutexes;

// Function to render a point (fragment) on the framebuffer
void point(Fragment f);

// Function to clear the framebuffer with the background color and depth
void clearFramebuffer();

// Function to render the framebuffer onto the SDL renderer
void renderBuffer(SDL_Renderer* renderer);