#version 460 core

struct Material
{
    int materialBindingIndex; // Stores the input of GL_TEXTURE_0
    int temp1;
    int temt2;
    int temp3;
};

layout(std430, binding = 1) buffer Materials {
    Material materials[];
};

in vec2 vTexCoord;
in vec3 vNormal;
out vec4 FragColor;
flat in int materialIndex;

//uniform sampler2D u_Diffuse;

uniform sampler2D uTextures[32];

void main()
{
    if(materialIndex == -1)
    {
        FragColor = vec4(1.0, 1.0, 0.0, 1.0);
    }
    else
    {
        int texUnit = materials[materialIndex].materialBindingIndex;
        FragColor = texture(uTextures[texUnit], vTexCoord);
    }
    
    //FragColor = texture(u_Diffuse, vTexCoord);
    
}
