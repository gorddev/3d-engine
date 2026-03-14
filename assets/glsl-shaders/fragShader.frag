#version 410 core
precision mediump float;

in vec2 vUV;
in vec3 vNormal;

out vec4 color;
void main() {
    color = vec4(0.0, 0.0, 0.0, 1.0);
}

precision mediump float;

in vec2 vUV;
in vec3 vNorm;
in float time;
in vec2 mousePos;



out vec4 color;

void main() {
    vec2 mp = mousePos;
    float t = time/1000.0;
    vec2 norm = normalize(gl_FragCoord.xy - mp.xy);
    color = vec4(norm, sin(time), 1.0);
}