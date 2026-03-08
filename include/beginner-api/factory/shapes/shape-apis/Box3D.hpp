#pragma once

// made by gordie novak feb 22nd 2026

// the box3d api is the ShapeAPI implementation for a box!

#include <beginner-api/factory/shapes/ShapeAPI.hpp>
#include <beginner-api/factory/shapes/raw-shapes/Box3DRaw.hpp>

namespace gan {

    struct Box3D : ShapeAPI {
        using SisterType = Box3DRaw;

        /// Width, height, and length of the Box3D
        float& w, h, l;

        /// Constructor for the Box3DAPI.
        explicit Box3D(const Model& model)
            : ShapeAPI(model),
                w(scale.x),
                h(scale.y),
                l(scale.z) {}

        /// Makes a new box from the old one
        [[nodiscard]] Box3D clone() const {
            return Box3D{Model(*this)};
        }

        [[nodiscard]] glm::vec3 getTransform() const {
            return this->scale;
        }

    };

}
