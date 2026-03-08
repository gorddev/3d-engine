#pragma once

#include <cstdint>

// Created by Gordie Novak on 2/17/26.

// vertex buffers hold all the information needed
// to render an object that has already been added
// to open gl.

typedef uint32_t GLuint; //< typedef so we can use w/o includes.

namespace gan::gl {

    /// A vertex buffer is an internal engine object
    /// that stores internal vertex and rendering information
    /// for openGL rendering.
    struct VertexBuffer {
        GLuint vao = 0;     ///< Vertex Array handle
        GLuint vbo = 0;     ///< Vertex Buffer handle
        GLuint ebo = 0;     ///< Index buffer handle
        uint16_t indexCount = 0;  ///< Number of indices
    };

}