#pragma once

#ifdef ENGINE_EXPORTS
#define ENGINE_API __declspec(dllexport)
#else
#define ENGINE_API __declspec(dllimport)
#endif

#include <glm/glm.hpp>

enum class GLTFLightType : int
{
    NoLight = 0,
    Directional = 1,
    Ending = 2 // Its used to represent the light array have ended. Its can be used for traversal in shader
};

class ENGINE_API LightingComponent
{
public:
    GLTFLightType lightType;
    glm::vec3 position;
    glm::vec3 lightColor; //Diffue
    float ambientStrength;
    float diffuseStrength;
    float specularStrength;
	
    LightingComponent() = default;
    LightingComponent(
        GLTFLightType gltfLightType,
        glm::vec3 position,
        glm::vec3 lightColor,
        float ambientStrength,
        float diffuseStrength,
        float specularStrength
    );
};


