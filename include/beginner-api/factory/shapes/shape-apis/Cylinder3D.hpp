#pragma once

// made by gordie novak feb 22nd 2026

// the box3d api is the ShapeAPI implementation for a box!

#include <beginner-api/factory/shapes/ShapeAPI.hpp>

#include "beginner-api/factory/shapes/raw-shapes/Cylinder3DRaw.hpp"
#include "beginner-api/factory/shapes/raw-shapes/Prism3DRaw.hpp"

namespace gan {

    struct Cylinder3D : ShapeAPI {
        template<size_t N>
        using SisterType = Cylinder3DRaw<N>;

        static constexpr int DefaultDetail = 12ul;

        /// Width, height, and length of the Box3D
        float& rad, l;

        /// Constructor for the Box3DAPI.
        explicit Cylinder3D(const Model& model)
            : ShapeAPI(model),
                rad(scale.x),
                l(scale.z) {}

    };

}
