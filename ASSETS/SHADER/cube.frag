#version 460 core

in vec2 vTexCoord;
out vec4 FragColor;

uniform sampler2D u_Diffuse;

void main()
{
    FragColor = texture(u_Diffuse, vTexCoord);
    //FragColor = vec4(1.0, 1.0, 0.0, 1.0);
}
