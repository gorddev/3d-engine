#include <rendering/shaders/Shader.hpp>

namespace gan {

    inline constexpr char TextureColor2DFragShader[] =
#ifdef __EMSCRIPTEN__
R"(#version 300 es)"
#else
R"(#version 410 core)"
#endif
R"(
precision mediump float;

in vec2 vUV;
in vec3 vNorm;

out vec4 FragColor;

void main()
{
    vec4 norm = normalize(gl_FragCoord);
    norm.a = 1.0;
    FragColor = norm;
}
)";

    inline constexpr std::initializer_list<UniformDesc>
        TextureColor2DFragUniforms = {};

}