#pragma once
#include <glm/vec2.hpp>

#include "dim2.hpp"

/* Created by Gordie Novak on 3/13/26.
 * Purpose: 
 * a rectangle, except uses floats for all its values. */

namespace gan {

    struct fRect {
        float x, y, w, h;

        static float area(const fRect& r) {
            return r.w*r.h;
        }
        static glm::vec2 pos(const fRect& r) {
            return glm::vec2{r.x, r.y};
        }
        static dim2 dim(const fRect& r) {
            return {static_cast<uint16_t>(r.x), static_cast<uint16_t>(r.y)};
        }
        static void move(fRect& r, const float x, const float y) {
            r.x += x;
            r.y += y;
        }
        /** Returns the frect in normalized device coordinates */
        static fRect ndc(const fRect& r, const dim2& dim) {
            return fRect {
                (r.x*2.f/dim.w) - 1.f,
                (r.y*2.f/dim.h) - 1.f,
                (r.w*2.f/dim.w) - 1.f,
                (r.h*2.f/dim.h) - 1.f
            };
        }
    };

}