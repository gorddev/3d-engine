#pragma once
#include <cstdint>
#include <glm/vec4.hpp>

#include "../ShapeUtils.hpp"
#include "rendering/render-primitives/Vertex.hpp"

// made by gordie novak on feb 21st, 2026
// the rectangle is just like a square, but it's a rectangle!

namespace gan {

    struct Box3DRaw {
        /// The number of vertices that make up a rectangle.
        static constexpr size_t numVertices = 36;

        /// Constexpr vertex creation method.
        static constexpr const Vertex* vertices() noexcept {
            static const Vertex rectVertices[numVertices] = {
                // Front (+Z)
                {-1,-1, 1},
                { 1,-1, 1},
                { 1, 1, 1},
                {-1,-1, 1},
                {1, 1, 1},
                {-1, 1, 1},

                // Back (-Z)
                {1,-1,-1},
                {-1,-1,-1},
                {1,-1,-1},
                {1,-1,-1},
                {-1, 1,-1},
                {1, 1,-1},

                // Left (-X)
                {-1,-1,-1},
                {-1,-1, 1},
                {-1, 1, 1},
                {-1,-1,-1},
                {-1, 1, 1},
                {-1, 1,-1},

                // Right (+X)
                {1,-1, 1},  {1,-1,-1},  {1, 1,-1},
                {1,-1, 1},  {1, 1,-1},  {1, 1, 1},

                // Top (+Y)
                {-1, 1, 1},  {1, 1, 1},  {1, 1,-1},
                {-1, 1, 1},  {1, 1,-1}, {-1, 1,-1},

                // Bottom (-Y)
                {-1,-1,-1},  {1,-1,-1},  {1,-1, 1},
                {-1,-1,-1},  {1,-1, 1}, {-1,-1, 1}
            };
            return rectVertices;
        }

        static constexpr glm::vec3 scaleFactor(float w, float h, float l) {
            return glm::vec3{w, h, l};
        }

    };


}