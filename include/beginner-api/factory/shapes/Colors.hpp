#pragma once
#include <glm/vec4.hpp>

// made by gordie novak on feb 21st, 2023

// used for easy indexing and access of various colors, so you
// don't have to manually specify raw colors each time

namespace gan {

    using RGBAVal = glm::vec4;

    // Basic color definitions for easy access of colors. 
    constexpr RGBAVal red     = {1.f, 0.235f, 0.235f, 1.f};   ///< A 4f representation for red
    constexpr RGBAVal orange  = {1.f, 0.65f, 0.f, 1.f};       ///< A 4f representation for orange
    constexpr RGBAVal yellow  = {1.f, 1.f, 0.235f, 1.f};      ///< A 4f representation for yellow
    constexpr RGBAVal green   = {0.4f, 0.8f, 0.4f, 1.f};      ///< A 4f representation for green
    constexpr RGBAVal blue    = {0.475f, 0.67f, 1.f, 1.f};    ///< A 4f representation for blue
    constexpr RGBAVal purple  = {0.8f, 0.35f, 1.f, 1.f};      ///< A 4f representation for purple. 
    // Extra fancier colors.
    constexpr RGBAVal light_blue = {0.55f, 0.9f, 1.f, 1.f};   ///< A 4f representation for light_blue
    constexpr RGBAVal pink = {1.f, 0.67f, 0.67f, 1.f};        ///< A 4f representation of pink
    constexpr RGBAVal white = {0.9f, 0.9f, 0.9f, 0.9f};       ///< A 4f representation of white
    constexpr RGBAVal black = {0.075f, 0.075f, 0.075f, 1.f};  ///< A 4f representation for black.


    /// This namespace is used for raw colors and other constants that give exact values
    namespace exact {
        constexpr RGBAVal red = {1.f, 0.f, 0.f, 1.f};     ///< Exact 4f representation of pure red
        constexpr RGBAVal green = {0.f, 1.f, 0.f, 1.f};   ///< Exact 4f representation of pure green
        constexpr RGBAVal blue = {0.f, 0.f, 1.f, 1.f};    ///< Exact 4f representation of pure blue
        constexpr RGBAVal magenta = {1.f, 0.f, 1.f, 1.f}; ///< Exact 4f representation of pure magenta
        constexpr RGBAVal yellow = {1.f, 1.f, 0.f, 1.f};  ///< Exact 4f representation of pure yellow
        constexpr RGBAVal cyan = {0.f, 1.f, 1.f, 1.f};    ///< Exact 4f representation of pure cyan
        constexpr RGBAVal white = {1.f, 1.f, 1.f, 1.f};   ///< Exact 4f representation of pure white.
        constexpr RGBAVal black = {0.f, 0.f, 0.f, 1.f};   ///< Exact 4f representation of pure black
        constexpr RGBAVal transparent = RGBAVal(0.f);        ///< Exact 4f representation of pure transparency.
        
    }// namespace exact
}// namespace gan