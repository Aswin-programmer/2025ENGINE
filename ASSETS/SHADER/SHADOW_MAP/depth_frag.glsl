#version 460 core

struct Material
{
    int materialBindingIndex; // Stores the input of GL_TEXTURE_0
    float ambientStrength;
    float diffuseStrength;
    float specularStrength;
};

layout(std430, binding = 1) buffer Materials {
    Material materials[];
};

struct Light
{
    vec3 position;
    int gltfLightType;
    vec3 lightColor;
    float ambientStrength;
    float diffuseStrength;
    float specularStrength;

    float constant; // For the PointLight
    float linear;   // For the PointLight

    vec3 direction; // For the spotlight[values between 0-1]

    float quadratic;// For the PointLight

    
    float cutOff;
};

layout(std430, binding = 3) buffer Lights
{
    Light lights[];
};

in vec2 vTexCoord;
in vec3 vNormal;
out vec4 FragColor;
flat in int materialIndex;

uniform sampler2D uTextures[32];
uniform vec3 viewPos; // camera world position

in vec3 worldPosition;

void main()
{
    
}
