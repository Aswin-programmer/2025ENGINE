#include "LightingComponent.h"

LightingComponent::LightingComponent(
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
)
	:
	lightType{ gltfLightType },
	position{ position },
	lightColor{ lightColor },
	ambientStrength{ ambientStrength },
	diffuseStrength{ diffuseStrength },
	specularStrength{ specularStrength },
	constant{constant},
	linear{linear},
	quadratic{quadratic},
	direction{direction},
	cutOff{cutOff}
{
}
