#version 460 core

in vec2 vTexCoord;

layout (binding = 0) uniform sampler2D uTexture;

out vec4 FragColor;

void main()
{
    FragColor = texture(uTexture, vTexCoord);
}