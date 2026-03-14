#pragma once
#include "VertexBuffer.hpp"

namespace gan {

    /// Holds a vertexBuffer and a material
    struct Mesh {
        GLuint shader;      ///< Shader used to render the mesh.
        uint32_t tex_id;    ///< Pointer to the linked texture
    };
}
