#pragma once
#include <cstdint>
#include <glm/vec4.hpp>

#include "../ShapeUtils.hpp"
#include "rendering/glPrimitives/glVertex.hpp"

// made by gordie novak on feb 21st, 2026
// the rectangle is just like a square, but it's a rectangle!

namespace gan {

    struct Prism3DRaw {
        /// The number of vertices that make up a rectangle.
        static constexpr size_t numVertices = 5;
        /// The number of indices used to index the vertices of the rectangle.
        static constexpr size_t numIndices = 18;
        /// Ensures that the Rect3D is properly set up.
        static unsigned int constexpr prismIndices[] = {
            0,1,4, 1,2,4, 2,3,4, 3,0,4,
            0,1,2, 0,2,3
        };

        /// Constexpr vertex creation method.
        static constexpr const gl::vertex3* vertices() noexcept {
            static const gl::vertex3 prismVertices[5] = {
                { 1.f, -1.f,  1.f, 1.0, 1.0, 1.0, 1.0},
                { 1.f, -1.f, -1.f, 1.0, 1.0, 1.0, 1.0},
                {-1.f,  -1.f, -1.f, 1.0, 1.0, 1.f, 1.0},
                { -1.f, -1.f, 1.f, 1.0, 1.0, 1.0, 1.0},
                { 0.f,  1.f,  0.f, 1.0, 1.0, 1.0, 1.0},
            };
            return prismVertices;
        }

        static constexpr std::array<unsigned int, numIndices> indices() noexcept {
            std::array<unsigned int, numIndices> indexArr{};
            // copy the constant indices into our indexes array.
            memcpy(indexArr.data(), prismIndices, numIndices * sizeof(uint32_t));
            // return our indices.
            return indexArr;
        }

        static constexpr glm::vec3 scaleFactor(float w, float h, float l) {
            return glm::vec3{w, h, l};
        }

    };


}