#pragma once
#include <optional>

#include "types/enums/RenderingSettings.hpp"

// made by gordie feb 20th

// used to abstract away a lot of the stuff involved in creating
// textures and stuff for gl specifically.

using GLuint = uint32_t;

namespace gan {

    namespace gl {
        /// Generates a 2D textrue from the given parameters.
        /// @param pixels A raw pointer to the pixels of the surface.
        /// @param w The width of the surface
        /// @param h The height of the surface
        /// @param scaleMode
        /// @param format The format of the texture (RGBA, BGRA, ARGB, ect.)
        /// @param type The actual data type used to represent the colors (@code GL_UNSIGNED_BYTE@endcode, @code GL_FLOAT@endcode, ect.)
        /// @return GLuint if texture generation was successful. @code std::nullopt@endcode otherwise.
        std::optional<GLuint> generate2DTexture(
            const void *pixels,
            uint32_t w,
            uint32_t h,
            ScaleMode scaleMode,
            GLuint format = 0x1908, //< GL Macro for rgba
            GLuint type = 0x1401); //< GL macro for unsigned byte

    }

}