#include "LightingComponent.h"

LightingComponent::LightingComponent(
	GLTFLightType gltfLightType, 
	glm::vec3 position, 
	glm::vec3 lightColor, 
	float ambientStrength, 
	float diffuseStrength, 
	float specularStrength
)
	:
	lightType{ gltfLightType },
	position{ position },
	lightColor{ lightColor },
	ambientStrength{ ambientStrength },
	diffuseStrength{ diffuseStrength },
	specularStrength{ specularStrength }
{
}
