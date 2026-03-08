#version 410 core
precision mediump float;

in vec2 vUV;
in vec3 vNormal;

out vec4 color;
void main() {
    color = vec4(0.0, 0.0, 0.0, 1.0);
}