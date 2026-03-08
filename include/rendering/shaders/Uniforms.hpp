#pragma once
#include <string>
#include <variant>

#include "glm/fwd.hpp"
#include "glm/vec2.hpp"

// made by gordie feb 23rd. contains all the information
// needed to make uniform updating easy & simple.

typedef int GLint;

namespace gan {




    /// Contains the data type the uniform represents
    enum UniformType {
        // Regular Uniforms
        Float, Vec2, Vec3, Vec4, Mat4, Sampler2D,

        // Auto Uniforms (Special)
        $SPC$, //< special uniform marker
        AutoCamPerspective, ///< Automatically update with Camera perspective mat4
        AutoCamView,        ///< Automatically update with Camera view mat4
        AutoTime,           ///< Automatically update with global time (float)
        AutoDeltaTime,      ///< Automatically update with global delta time (float)
        AutoFrame,          ///< Automatically update with global frame number (int)
        AutoMousePos,       ///< Automatically update with global mouse pos. (float)
        // <><> Per-Object Updates <><>
        AutoModel,          ///< Automatically update with model mat4
        AutoTexture,        ///< Automatically update with model texture (sampler2D)
        AutoColor,          ///< Automatically applies color.

    };

    /// The actual uniform struct.
    struct Uniform {
        GLint location;
        UniformType type;
        std::string name;
    };

    /// Descriptor for a uniform.
    /// Contains the name of the uniform and type.
    struct UniformDesc {
        UniformType type;
        std::string name;
    };

}// namespace gan