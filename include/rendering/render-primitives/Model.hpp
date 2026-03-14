#pragma once
#include <rendering/render-primitives/Transform.hpp>
#include "rendering/render-primitives/Mesh.hpp"
#include "rendering/render-primitives/VertexBuffer.hpp"

// made by gordie feb 21st.

namespace gan {

    struct Model {
        VertexBuffer vb;    ///< Underlying links to openGL vertex buffer.
        Mesh mesh;          ///< Contains link to texture & shader program.
        Transform t;        ///< Contains position, rotation, scale information.
    };

}