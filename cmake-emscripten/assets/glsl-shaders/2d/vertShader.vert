#version 300 es
precision mediump float;

layout(location = 0) in vec2 aPos;
layout(location = 1) in vec2 aTexCoords;
layout(location = 2) in vec4 aColor;

out vec2 vUV;
out vec4 vColor;

void main() {
    vUV = aTexCoords;
    vColor = aColor;
}