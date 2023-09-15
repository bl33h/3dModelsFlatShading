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
#include <array>
#include <algorithm>
#include <glm/glm.hpp>
#include <limits>
#include <mutex>
#include "colors.h"
#include "fragment.h"
#include "vertex.h"
#include <SDL2/SDL.h>

constexpr size_t SCREEN_WIDTH = 800;
constexpr size_t SCREEN_HEIGHT = 600;

extern FragColor blank;
extern std::array<FragColor, SCREEN_WIDTH * SCREEN_HEIGHT> framebuffer;
extern std::array<std::mutex, SCREEN_WIDTH * SCREEN_HEIGHT> mutexes;

void point(Fragment f);
void clearFramebuffer();
void renderBuffer(SDL_Renderer* renderer);
