#version 300 es
precision mediump float;

in vec2 vUV;
in vec4 vColor;

out vec4 FragColor;

void main()
{
    FragColor = texture(uTex, vUV) * vColor;
}