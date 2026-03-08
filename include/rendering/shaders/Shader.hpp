#pragma once

#include <array>
#include <optional>
#include <utility>

#include "Uniforms.hpp"
#include "core/EngineCore.hpp"
#include "../../scene/Camera.hpp"


// made by gordie novak on feb 23rd
// handles management and uniform upload for a single shader.

typedef uint32_t GLuint;
typedef int GLint;

namespace gan {

    /// The maximum number of uniforms a shader is allowed to have.
    constexpr uint16_t maxShaderUniforms = 10;

    /// Class used to manage upload of shader uniforms and
    /// batching for various shader calls.
    class Shader { // NOLINT(*-pro-type-member-init)
        /// Program ID of the shader
        GLuint program = 0;
        /// Each of the uniforms stored in a vector
        std::vector<Uniform> userUniforms;
        /// Each of the special uniforms stored in their own vector
        std::vector<Uniform> reservedUniforms;

        // private default constructor.
        Shader() = default;

        /// Updates all the engine-managed uniforms in the Shader
        void updatePerFrameUniforms(const EngineCore& core, const Camera& cam);

        /// Returns true if this shader binds Model Matrices
        [[nodiscard]] bool requiresModelMatrix() const;
        /// Returns true if this shader binds to textures per model.
        [[nodiscard]] bool requiresTexture() const;
        /// Gets the underlying openGL program so you can add unforms
        [[nodiscard]] GLuint getGLProgram() const;

        friend class ShaderHandler;
        friend class Renderer;

    public:
        /// Creates a Shader from the specified parameters.
        /// @param program The OpenGL program that will be held by this shader.
        /// @param uniforms List of uniform descriptors.
        /// @return A @code Shader@endcode if the shader creation was successful, a @code std::nullopt@endcode if not.
        static std::optional<Shader> make(GLuint program,
            std::initializer_list<UniformDesc> uniforms = {});

    };

}