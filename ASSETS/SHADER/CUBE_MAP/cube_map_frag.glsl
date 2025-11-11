#version 460 core
in vec3 TexCoords;
out vec4 FragColor;

uniform samplerCube skybox; // bound to texture unit 0

void main()
{
    vec4 color = texture(skybox, TexCoords);
    //FragColor = vec4(0.f, 0.f, 1.f, 1.f);
    FragColor = color;
}
