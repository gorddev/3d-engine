#pragma once

#include "./Shader.hpp"
#include "types/custom-data-structures/map_vector.hpp"
#include "core/filesystem/filesystem.hpp"

// made by gordie novak, feb 22nd, 2023

// class used for managing built-in shaders and default shaders

namespace gan {

    /// Enumerated class for default shader programs.
    enum class ShaderID : uint32_t {
        BASIC_3D = 0,
        BASIC_2D = 1
    };
    inline constexpr int numDefaultShaders = 2;


    typedef uint32_t GLuint; ///< Used for shader values.
    class Renderer;

    /// Holds links to each of the different shader types as
    /// well as each of the default shader programs.
    class ShaderHandler {
        /// User-created shader programs
        map_vector<std::string, Shader> shaderPrograms;
        /// Default shader programs
        GLuint default3DProgram = 0, default2DProgram = 0;
        /// Current shader program
        GLuint currentProgram_id = 0;

        friend class gan::Renderer; ///< Renderer needs special privileges to stuff in the ShaderManager.
        friend class EngineCore;

        /// Ensures that the current shader name is not in use.
        void verifyShaderName(const std::string& shaderName);

        void init();
    public:
        /// Constructor compiles all the static shader programs.
        ShaderHandler();

        /// Binds the current 3D shader program for new object creation
        /// @param shader The static pre-compiled shader you want to use.
        void bind3DShader(ShaderID shader);
        /// Binds a user-defined 3D shader for new object creation.
        /// @param shaderName The alias/name of the shader you have created.
        void bind3DShader(const std::string &shaderName);
        /// Binds the current 2D shader program for new object creation
        /// @param shader The static pre-compiled shader you want to use.
        void bind2DShader(ShaderID shader);
        /// Binds a user-defined 2D shader for new object creation.
        /// @param shaderName The alias/name of the shader you have created.
        void bind2DShader(const std::string &shaderName);

        /// Creates an OpenGL shader program from the provided shaders along the paths.
        /// @param shaderName The name you want to bind to the shader.
        /// @param vertexShaders The paths to the vertex shader strings
        /// @param fragmentShaders The paths to the fragment shader strings.
        /// @param userUniforms Sets up uniforms to be edited and updated automatically.
        void makeShader(const std::string &shaderName,
                std::initializer_list<path> vertexShaders,
                std::initializer_list<path> fragmentShaders,
                std::initializer_list<UniformDesc> userUniforms);

        /// Creates a shader from a user-compiled program. Unsafe.
        /// @warning Only pass in your own defined shader that you have
        /// compiled and linked manually with openGL.
        void _instantiateShader(const std::string &shaderName, GLuint);

        /// Gets the default 2D shader currently set. Used for creating new objects.
        [[nodiscard]] GLuint getDefault2DShader() const {
            return err::unwrap(getShaderByIndex(default2DProgram),
                "ShaderManager::getDefault2DShader()", "Shader does not exist.")
                ->program;
        }
        /// Gets the default 3D shader currently set. Used for creating new objects.
        [[nodiscard]] GLuint getDefault3DShader() const {
            return err::unwrap(getShaderByIndex(default2DProgram),
                "ShaderManager::getDefault3DShader()", "Shader does not exist.")
                ->program;
        }

        /// Gets the base shader of the shaderManager
        [[nodiscard]] GLuint base() {
            return shaderPrograms.at(0).value()->getGLProgram();
        }

        /// Gets a shader by program_id
        [[nodiscard]] std::optional<Shader*> getShaderByProgramID(GLuint program_id) {
            for (auto& shader : shaderPrograms) {
                if (shader.getGLProgram() == program_id) {
                    return &shader;
                }
            }
            GAN_WriteLog("ShaderManager::getShaderByProgramID()",
                    "failed to retrieve program, ", program_id, " as it does not exist.");
            return std::nullopt;
        }

        // ensure it can't be moved/copied
        ShaderHandler(ShaderHandler&&)                  = delete;   ///< Nope. You can't do that.
        ShaderHandler(const ShaderHandler&)             = delete;   ///< Nope. You can't do that.
        ShaderHandler operator=(const ShaderHandler&)   = delete;   ///< Nope. You can't do that.
        ShaderHandler operator=(ShaderHandler&&)        = delete;   ///< Nope. You can't do that.

    private:
        /// Gets a shader by index
        [[nodiscard]] std::optional<Shader*> getShaderByIndex(GLuint index) const {
            const auto s =  shaderPrograms.get(index);
            if (!s) {
                GAN_WriteLog("ShaderManager::getShaderByIndex()",
                    "Shader program ", static_cast<int>(index), " not found.");
            }
            return s;
        }
        /// Updates per-frame uniforms
        void updateFrameUniforms(EngineCore& core, Camera& cam);
        /// Updates global uniforms for all shaders
        void updateGlobalUniforms(EngineCore& core);
    };



}