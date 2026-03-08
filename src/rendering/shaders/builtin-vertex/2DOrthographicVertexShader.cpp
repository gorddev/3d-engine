
namespace gan {

    inline constexpr char Orthographic2DVertexShader[] =
#ifdef __EMSCRIPTEN__
R"(#version 300 es)"
#else
R"(#version 410 core)"
#endif
R"(
precision mediump float;

layout(location = 0) in vec2 aPos;
layout(location = 1) in vec2 aTexCoords;
layout(location = 2) in vec3 aNorm;

out vec2 vUV;
out vec3 vNorm;

void main() {
    vUV = aTexCoords;
    gl_Position = vec4(aPos, 0.0, 1.0);
})";
}