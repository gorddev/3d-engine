#pragma once
#include "glVertexBuffer.hpp"

namespace gan::gl {

    /// Holds texture handle for a texture managed by OpenGL
    struct texture {
        GLuint id = 0;
        uint32_t width = 0;
        uint32_t height = 0;
    };

    /// Material used by a mesh to render itself.
    struct material {
        GLuint shader;              ///< Shader used to render the mesh.
        uint32_t gl_tex_id;         ///< Pointer to the linked texture
    };

    /// Holds a vertexBuffer and a material
    struct mesh {
        VertexBuffer buf;  ///< Pointer to a representative VertexBuffer
        GLuint shader;     ///< Shader used to render the mesh.
        uint32_t gl_tex_id;///< Pointer to the linked texture
    };
}
