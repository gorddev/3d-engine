#include <rendering/shaders/ShaderManager.hpp>

#include "../../../include/core/debug/gan_err.hpp"
#include <rendering/glFunctions/glShaders.hpp>
#include <core/filesystem/filesystem.hpp>
#include <utility>

// include the raw string literals from .cpp files
#include "builtin-fragment/3DColorFragShader.cpp"
#include "builtin-fragment/2DTextureColorFragShader.cpp"
#include "builtin-vertex/2DOrthographicVertexShader.cpp"
#include "builtin-vertex/3DPerspectiveVertexShader.cpp"

using namespace gan;

void ShaderHandler::verifyShaderName(const std::string &shaderName) {
    // Panics if the shader name already exists.
    if (shaderPrograms.has(shaderName))
        err::panic("ShaderManager::createShader()",
            "A shader with the name ", shaderName.c_str(), " already exists!");
}

ShaderHandler::ShaderHandler() = default;

// loads all the default shaders.
void ShaderHandler::init() {
    auto program = err::unwrap(gl::rawMakeShaderProgram(
        {{Perspective3DVertexShader, sizeof(Perspective3DVertexShader) - 1}},
        {{Color3DFragShader, sizeof(Color3DFragShader) - 1}}
    ), "ShaderManager::init()");
    // first make our raw 3d shader program.
    shaderPrograms.add("BASIC_3D",
        err::unwrap(Shader::make(program, Perspective3DVertexUniforms), "ShaderManager::init() {0}"));

    // next make the basic 2d shader program.
    program = err::unwrap(gan::gl::rawMakeShaderProgram(
        {{Orthographic2DVertexShader, sizeof(Orthographic2DVertexShader)}},
        {{TextureColor2DFragShader, sizeof(TextureColor2DFragShader)}}
    ), "ShaderManager::init()");
    shaderPrograms.add("BASIC_2D",
        err::unwrap(Shader::make(program), "ShaderManager::init() {1}"));

    // set our default programs.
    default3DProgram = static_cast<int>(ShaderID::BASIC_3D);
    default2DProgram = static_cast<int>(ShaderID::BASIC_3D);

}

void ShaderHandler::bind3DShader(ShaderID shader) {
    default3DProgram = static_cast<int>(shader);
}

void ShaderHandler::bind3DShader(const std::string &shaderName) {
    auto opt = shaderPrograms.map(shaderName);
    if (opt.has_value())
       default3DProgram = opt.value()->program;
    else {
        err::panic("ShaderManager::bind3DShader()",
            "Cannot bind shader \"", shaderName.c_str(),
            "\" because it does not exist.");
    }
}

void ShaderHandler::bind2DShader(ShaderID shader) {
    default3DProgram = static_cast<int>(shader);
}

void ShaderHandler::bind2DShader(const std::string &shaderName) {
    auto opt = shaderPrograms.map(shaderName);
    if (opt.has_value())
        default2DProgram = opt.value()->program;
    else {
        err::panic("ShaderManager::bind3DShader()",
            "Cannot bind shader ", shaderName.c_str(),
            " because it has not been created");
    }
}


void ShaderHandler::makeShader(const std::string &shaderName,
     const std::initializer_list<path> vertexShaders,
     const std::initializer_list<path> fragmentShaders,
     std::initializer_list<UniformDesc> userUniforms)
{
    verifyShaderName(shaderName);
    // Create the program. Has the possibility to fail (err::unwrap).
    const auto program = err::unwrap(
        gl::makeShaderProgram(vertexShaders, fragmentShaders),
        "ShaderManager::makeShader()",
        "Failed to to compile shader '", shaderName.c_str(),
        "\' for file ", '\'', shaderName.c_str(), '\''
    );
    // Add the user program if everything's successful.
    shaderPrograms.add(shaderName,
        err::unwrap(Shader::make(program, userUniforms),
            "ShaderManager::makeShader()",
            "Failed to unwrap shader '", shaderName.c_str(), '\'')
    );
}

void ShaderHandler::_instantiateShader(const std::string &shaderName, GLuint program) {
    verifyShaderName(shaderName);
    // adds the program to the user program map.
    Shader s;
    s.program = program;
    shaderPrograms.add(shaderName, s);
}

void ShaderHandler::updateFrameUniforms(EngineCore& core, Camera &cam) {
    for (auto& s : shaderPrograms) {
        glUseProgram(s.program);
        currentProgram_id = s.program;
        s.updatePerFrameUniforms(core, cam);
    }
}