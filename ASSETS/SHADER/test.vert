#version 450 core
layout(location = 0) in vec2 aPos;
layout(location = 1) in vec2 aUV;
out vec2 vUV;
uniform float zoom;
void main() {
    vUV = aUV * zoom;
    gl_Position = vec4(aPos, 0.0, 1.0);
}