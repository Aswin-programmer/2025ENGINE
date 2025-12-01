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
    vec2 padding; //Temporary
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
uniform vec3 viewPos; // camera/world position

void main()
{
    vec3 color;

    vec3 N = normalize(vNormal);
    vec3 V = normalize(viewPos); // Assuming world space positions; adjust if needed

    vec3 result = vec3(0.0);

    if(materialIndex == -1)
    {
        color = vec3(0.0, 1.0, 1.0);
    }
    else
    {
        Material currentMaterial =  materials[materialIndex];
        int texUnit = currentMaterial.materialBindingIndex;
        color = texture(uTextures[texUnit], vTexCoord).rgb;

        // Iterate over lights until gltfLightType == Ending
        for(int i = 0; i < lights.length(); ++i)
        {
            Light L = lights[i];

            if(L.gltfLightType == 2) // Ending
                break;
            else if(L.gltfLightType == 1) // Directional Lighting
            {
                // Directional Light: assume position is direction
                vec3 lightDir = normalize(-L.position); // Directional lights point from light to object

                // Ambient
                vec3 ambient = L.ambientStrength * L.lightColor * currentMaterial.ambientStrength;

                // Diffuse
                float diff = max(dot(N, lightDir), 0.0);
                vec3 diffuse = diff * L.diffuseStrength * L.lightColor* currentMaterial.diffuseStrength;

                // Specular
                vec3 H = normalize(lightDir + V); // Blinn-Phong half-vector
                float spec = pow(max(dot(N, H), 0.0), 32.0); // shininess = 32
                vec3 specular = spec * L.specularStrength * L.lightColor* currentMaterial.specularStrength;

                // Accumulate
                result += ambient + diffuse + specular;
            }  
            else if(L.gltfLightType == 0)
            {
                continue;
            }
        }
    }

    FragColor = vec4(result * color, 1.0);
}
