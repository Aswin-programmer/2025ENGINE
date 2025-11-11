#version 460 core
layout(location = 0) in vec3 aPos;
uniform mat4 u_view;
uniform mat4 u_proj;

out vec3 TexCoords;

void main()
{
    // remove translation from the view matrix
    mat4 viewNoTrans = mat4(mat3(u_view)); // keep rotation only
    vec4 pos = u_proj * viewNoTrans * vec4(aPos, 1.0);
    gl_Position = pos;
    TexCoords = aPos;
}