#pragma once

namespace gan {

    inline constexpr char Color3DFragShader[] =
#ifdef __APPLE__
R"(#version 410 core)"
#else
R"(#version 300 es)"
#endif
R"(
precision mediump float;

in vec2 vUV;
in vec3 vNorm;

out vec4 color;

void main() {
    vec4 norm = normalize(gl_FragCoord);
    norm.a = 1.0;
    color = norm;
}
)";
}