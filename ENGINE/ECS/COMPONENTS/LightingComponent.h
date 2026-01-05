#pragma once

#ifdef ENGINE_EXPORTS
#define ENGINE_API __declspec(dllexport)
#else
#define ENGINE_API __declspec(dllimport)
#endif

#include <glm/glm.hpp>
#include <cereal/cereal.hpp>
#include <cereal/types/string.hpp>     // std::string
#include <cereal/types/vector.hpp>     // std::vector
#include <cereal/types/optional.hpp>

#include "../../SERIALIZATION/CerealGLM.h"

enum class GLTFLightType : int
{
    NoLight = 0,
    Directional = 1,
    PointLight = 2,
    SpotLight = 3,
    Ending = 4 // Its used to represent the light array have ended. Its can be used for traversal in shader
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
    float constant; // For the PointLight
    float linear;   // For the PointLight
    float quadratic;// For the PointLight
    glm::vec3 direction; // For the SpotLight[values in range 0-1]
    float cutOff; // For the StopLight
	
    LightingComponent() = default;
    LightingComponent(
        GLTFLightType gltfLightType,
        glm::vec3 position,
        glm::vec3 lightColor,
        float ambientStrength,
        float diffuseStrength,
        float specularStrength,
        float constant,
        float linear,
        float quadratic,
        glm::vec3 direction,
        float cutOff
    );

    template <class Archive>
    void serialize(Archive& ar)
    {
        ar(
            CEREAL_NVP(lightType),
            CEREAL_NVP(position),
            CEREAL_NVP(lightColor),
            CEREAL_NVP(ambientStrength),
            CEREAL_NVP(diffuseStrength),
            CEREAL_NVP(specularStrength),
            CEREAL_NVP(constant),
            CEREAL_NVP(linear),
            CEREAL_NVP(quadratic),
            CEREAL_NVP(direction),
            CEREAL_NVP(cutOff)
        );
    }
};      



