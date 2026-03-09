#pragma once

#include <rendering/shaders/Shader.hpp>

namespace gan {

    inline constexpr char Perspective3DVertexShader[] =
#ifdef __APPLE__
R"(#version 410 core)"
#else
R"(#version 300 es)"
#endif
R"(
precision mediump float;

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aUV;
layout (location = 2) in vec3 aNorm;
layout (location = 4) in mat4 iModel;

uniform mat4 uView;
uniform mat4 uProjection;

out vec2 vUV;
out vec3 vNorm;

void main()
{
    gl_Position = (uProjection * uView * iModel * vec4(aPos, 1.0));
    vNorm = aNorm;
    vUV = aUV;
})";

    inline const std::initializer_list<UniformDesc>
        Perspective3DVertexUniforms = {
        {AutoCamView, "uView"},
            {AutoCamPerspective, "uProjection"}
    };

}