/*---------------------------------------------------------------------------
Copyright (C), 2022-2023, Sara Echeverria (bl33h)
@author Sara Echeverria
FileName: colors.h
@version: I
Creation: 06/07/2023
Last modification: 16/09/2023
*Some parts were made using the AIs Bard and ChatGPT
------------------------------------------------------------------------------*/

#pragma once

#include <algorithm>
#include <iostream>

// Function template to clamp a value within a specified range
template <typename T>
constexpr const T& clamp(const T& value, const T& min, const T& max) 
{
    return (value < min) ? min : ((value > max) ? max : value);
}

// Struct representing a color with RGBA components
struct Color 
{
    unsigned char r; // Red component
    unsigned char g; // Green component
    unsigned char b; // Blue component
    unsigned char a; // Alpha component (transparency)

    // Constructor for creating a Color object with default values
    Color(unsigned char red = 0, unsigned char green = 0, unsigned char blue = 0, unsigned char alpha = 255)
        : r(clamp(red, static_cast<unsigned char>(0), static_cast<unsigned char>(255))),
          g(clamp(green, static_cast<unsigned char>(0), static_cast<unsigned char>(255))),
          b(clamp(blue, static_cast<unsigned char>(0), static_cast<unsigned char>(255))),
          a(clamp(alpha, static_cast<unsigned char>(0), static_cast<unsigned char>(255))) 
    {}

    // Overloaded addition operator for combining two colors
    Color operator+(const Color& other) const 
    {
        return Color(
            std::min(static_cast<int>(r) + other.r, 255),
            std::min(static_cast<int>(g) + other.g, 255),
            std::min(static_cast<int>(b) + other.b, 255)
        );
    }

    // Overloaded multiplication operator for scaling a color by a factor
    Color operator*(float factor) const 
    {
        return Color(
            clamp(static_cast<int>(r * factor), 0, 255),
            clamp(static_cast<int>(g * factor), 0, 75),
            clamp(static_cast<int>(b * factor), 0, 110)
        );
    }

    // Overloaded output stream operator for printing a Color object
    friend std::ostream& operator<<(std::ostream& os, const Color& color) 
    {
        os << "RGBA(" << static_cast<int>(color.r) << ", "
           << static_cast<int>(color.g) << ", "
           << static_cast<int>(color.b) << ", "
           << static_cast<int>(color.a) << ")";
        return os;
    }

    // Overloaded multiplication operator (scalar * Color)
    friend Color operator*(float factor, const Color& color) 
    {
        return color * factor;
    }
};