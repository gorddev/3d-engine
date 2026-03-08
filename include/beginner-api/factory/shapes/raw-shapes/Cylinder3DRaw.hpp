#pragma once

#include <array>
#include <cmath>
#include <cstdint>
#include <rendering/glPrimitives/glVertex.hpp>

#include "glm/vec3.hpp"

// made by gordie novak on feb 25th
// the cylinder is my first attempt at a templated raw shape
// hopefully it goes well and i make the vertices correctly.

namespace gan {

    template<size_t Detail> //< template specifies size
    struct Cylinder3DRaw {
        // vertices for top, bottom, and center of each side of the cylinder.
        static constexpr int numVertices = 2 * (Detail + 1);
        static constexpr int numIndices = 4 * Detail * 3;

        inline static gl::vertex3 vertexList[numVertices] = {};
        inline static std::array<unsigned int, numIndices> indexList = {};

        static const gl::vertex3* vertices() {
            static bool isInitialized = false;
            // if not initialized form the vertices.
            if (!isInitialized) {
                isInitialized = true;
                constexpr float angDiv = 2*3.14159265/((numVertices-2)/2);
                float currentAng = 0;
                for (int i = 0; i < (numVertices-2)/2; i++) {
                    vertexList[i] = gl::vertex3(
                        cosf(currentAng),
                        sinf(currentAng),
                        1.f
                    );
                    vertexList[i + (numVertices-2)/2] = gl::vertex3(
                        cosf(currentAng),
                        sinf(currentAng),
                        -1.f
                    );
                    currentAng += angDiv;
                }
                vertexList[numVertices-2] = gl::vertex3(
                    0.f, 0.f, 1.f, 0.f, 0.f
                );
                vertexList[numVertices-1] = gl::vertex3(
                    0.f, 0.f, -1.f, 0.f, 0.f);
            }
            return vertexList;
        }

        static const std::array<unsigned int, numIndices>& indices() {
            // top center
            static constexpr int t = numVertices - 2;
            // bottom center
            static constexpr int b = numVertices - 1;

            static bool isInitialized = false;
            if (!isInitialized) {

                static_assert(alignof(gl::vertex3) <= alignof(std::max_align_t));
                // first we get the bottom & top rendered.
                for (int i = 0; i < Detail; i++) {
                    indexList[i*3] = i;
                    indexList[(i*3 + 1)] = (i + 1)%Detail;
                    indexList[i*3+2] = t;

                    indexList[(i+Detail)*3] = i + Detail;
                    indexList[(i+Detail)*3+1] = (i + 1)%Detail +  Detail;
                    indexList[(i+Detail)*3+2] = b;

                }
                for (int i = Detail*2*3, k =0; i < numIndices - 5; i+=6, k++) {
                    indexList[i] = k;
                    indexList[i+1] = k + Detail;
                    indexList[i+2] = (k + 1)% Detail + Detail;
                    indexList[i+3] = k;
                    indexList[i+4] = (k + 1)%Detail;
                    indexList[i+5] = (k + 1)%Detail + Detail;
                }
            }
            return indexList;
        }

        static constexpr glm::vec3 scaleFactor(float rad, float l) {
            return glm::vec3{rad, rad, l};
        }
    };

}