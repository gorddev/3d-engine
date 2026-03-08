#pragma once
#include <cstdint>

namespace gan {

    using texID = uint32_t;
    using GLuint = uint32_t;

    /** Aggregate type representing a @code Texture@endcode.
     * - @code texID id@endcode › The unique ID of the texture [@code texID =  uint32_t@endcode].
     * - @code GLuint gl_id@endcode › The OpenGL handle to the texture.
     * - @code uint16_t w, h@endcode › Width and height of the texture. */
    struct Texture {
        texID id;
        GLuint gl_id;
        uint16_t w, h;
    };

}
