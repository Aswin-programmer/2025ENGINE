#version 460 core

layout (binding = 20) uniform sampler2D depthMap;

in vec2 TexCoord;

out vec4 FragColor;

void main()
{
    float depth = texture(depthMap, TexCoord).r;

    FragColor = vec4(vec3(depth), 1.0);
}
