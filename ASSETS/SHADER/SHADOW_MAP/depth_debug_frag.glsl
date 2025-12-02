#version 460 core

layout (binding = 0) uniform sampler2D depthMap;

in vec2 TexCoord;

out vec4 FragColor;

void main()
{
    float depth = texture(depthMap, TexCoord).r;

    // Optional: linearize depth for better visualization
    float near = 0.1;
    float far  = 100.0;
    float linearDepth = (2.0 * near) / (far + near - depth * (far - near));

    FragColor = vec4(vec3(linearDepth), 1.0);
}
