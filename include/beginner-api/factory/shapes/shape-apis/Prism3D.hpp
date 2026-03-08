#pragma once

// made by gordie novak feb 22nd 2026

// the box3d api is the ShapeAPI implementation for a box!

#include <beginner-api/factory/shapes/ShapeAPI.hpp>
#include "beginner-api/factory/shapes/raw-shapes/Prism3DRaw.hpp"

namespace gan {

    struct Prism3D final : ShapeAPI {
        using SisterType = Prism3DRaw;

        /// Width, height, and length of the Box3D
        float& w, h, l;

        /// Constructor for the Box3DAPI.
        explicit Prism3D(const Model& model)
            : ShapeAPI(model),
                w(scale.x),
                h(scale.y),
                l(scale.z) {}

        /// Makes a new box from the old one
        [[nodiscard]] Prism3D clone() const {
            return Prism3D{Model(*this)};
        }
    };

}
