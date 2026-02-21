#version 460 core

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec2 aTexCoord;

uniform mat4 uProjection;

out vec2 vTexCoord;

void main()
{
    vTexCoord = aTexCoord;

    // Convert pixel space ? clip space using orthographic matrix
    gl_Position = uProjection * vec4(aPosition, 1.0);
}