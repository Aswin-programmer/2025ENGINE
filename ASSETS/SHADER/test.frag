#version 450 core

in vec2 vUV;
out vec4 FragColor;

uniform sampler2DArray tex;

uniform int layer;

void main() {
    FragColor = texture(tex, vec3(vUV, layer));
    //FragColor = vec4(1.0, 1.0, 0.0, 1.f);
}