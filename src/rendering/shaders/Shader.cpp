
#include <rendering/shaders/Shader.hpp>
#include "../../../include/core/debug/gan_log.hpp"
#include <string>

#include "../../../include/core/debug/gan_err.hpp"
#include "core/EngineCore.hpp"
#include "glm/gtc/type_ptr.hpp"

using namespace gan;

std::optional<Shader> Shader::make(GLuint program, std::initializer_list<UniformDesc> userUniforms) {
    // error conditions
    if (userUniforms.size() > maxShaderUniforms) {
        gan::GAN_WriteLog("Shader::make()", "Number of provided uniforms",
            userUniforms.size(), ", is above maximum limit.");
        return std::nullopt;
    }
    if (program <= 0) {
        gan::GAN_WriteLog("Shader::make()", "The provided program",
            static_cast<int>(program), ", is invalid");
        return std::nullopt;
    }

    // make our shader
    Shader s;
    s.program = program;

    int count = 0, abcount = 0;
    // Use the program provided
    glUseProgram(program);
    for (auto& [type, name] : userUniforms) {
        const auto uniform_loc = glGetUniformLocation(program, name.c_str());
        // check to make sure the uniform is valid.
        if (uniform_loc <= -1) {
            GAN_WriteLog("Shader::Shader()", "The uniform specified by \'", name.c_str(),
                "\' at location ", static_cast<int>(uniform_loc), " does not exist within program ", (int)program, ".");
            return std::nullopt;
        }
        if (type >= UniformType::$SPC$) {
            s.reservedUniforms.push_back({uniform_loc, type});
        } else {
            s.userUniforms.push_back({uniform_loc, type, name});
        }
    }
    // Finally, return our shader.
    return s;
}

void Shader::updatePerFrameUniforms(const EngineCore& core, const Camera& cam) {
    glUseProgram(program);
    static glm::vec2 mp;
    static dim2 dim{0,0};
    for (const auto& u : reservedUniforms) {
        switch (u.type) {
        case AutoTime:
            glUniform1f(u.location, static_cast<float>(core.clock.time));
            break;
        case AutoDeltaTime:
            glUniform1f(u.location, static_cast<float>(core.clock.dt));
            break;
        case AutoFrame:
            glUniform1i(u.location, static_cast<int>(core.clock.frame));
            break;
        case AutoCamPerspective:
            glUniformMatrix4fv(u.location, 1,
            false, glm::value_ptr(cam.projection(core.window.getDimensions())));
            break;
        case AutoCamView:
            glUniformMatrix4fv(u.location, 1, false, glm::value_ptr(cam.view()));
            break;
        default:
            break;
        case AutoMousePos:
            mp = core.mouse.getPos();
            glUniform2f(u.location, mp.x, core.window.getHeight()-mp.y);
            break;
        case AutoResolution:
            dim = core.window.getDimensions();
            glUniform2f(u.location, dim.w, dim.h);
        }
    }
}

bool Shader::requiresModelMatrix() const {
    return std::ranges::any_of(reservedUniforms.begin(), reservedUniforms.end(),
        [](const Uniform& u) {
                return u.type == AutoModel;
        }
    );
}

bool Shader::requiresTexture() const {
    return std::ranges::any_of(reservedUniforms.begin(), reservedUniforms.end(),
        [](const Uniform& u) {
                return u.type == AutoTexture;
        }
    );
}

GLint Shader::getTexLocation() const {
    for (auto& u : reservedUniforms) {
        if (u.type == AutoTexture)
            return u.location;
    }
    return 0;
}

GLuint Shader::getGLProgram() const {
    return program;
}
