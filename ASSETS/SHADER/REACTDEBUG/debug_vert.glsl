#version 460 core

layout(location = 0) in vec3 aPosition;
layout(location = 1) in uint aColor;

uniform mat4 view;
uniform mat4 projection;

out vec4 vColor;

vec4 unpackColor(uint c)
{
    return vec4(
        float((c >> 24) & 0xFF) / 255.0,
        float((c >> 16) & 0xFF) / 255.0,
        float((c >> 8)  & 0xFF) / 255.0,
        float(c & 0xFF) / 255.0
    );
}

void main()
{
    vColor = unpackColor(aColor);
    gl_Position = projection * view * vec4(aPosition, 1.0);
}
