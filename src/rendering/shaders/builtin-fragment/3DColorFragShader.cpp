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
in vec4 vColor;

uniform sampler2D uTex;

out vec4 color;

void main() {
    color = texture(uTex, vUV);
}
)";
}