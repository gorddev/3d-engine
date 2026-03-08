#pragma once
#include "Transform.hpp"
#include "rendering/glPrimitives/glMesh.hpp"
#include "rendering/glPrimitives/glVertexBuffer.hpp"

// made by gordie feb 21st.

namespace gan {

    class Model {
    public:
        /// Underlying links to openGL vertex buffer.
        gl::VertexBuffer vb;
        /// Contains link to texture & shader program.
        gl::mesh mesh{};
        /// Contains position, rotation, scale information.
        Transform t;
    };

}