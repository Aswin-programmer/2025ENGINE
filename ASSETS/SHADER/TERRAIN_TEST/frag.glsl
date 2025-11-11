#version 450 core
out vec4 color;

uniform sampler2D tex_color;

in TES_OUT {
    vec2 tc;
} fs_in;

void main() {
    color = texture(tex_color, fs_in.tc);
}
