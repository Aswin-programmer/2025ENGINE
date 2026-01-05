#pragma once

#ifdef ENGINE_EXPORTS
#define ENGINE_API __declspec(dllexport)
#else
#define ENGINE_API __declspec(dllimport)
#endif

#include <glm/glm.hpp>
#include <cereal/cereal.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/optional.hpp>

#include "../../SERIALIZATION/CerealGLM.h"

struct ENGINE_API TransfromComponent
{
public:
	TransfromComponent() = default;
	TransfromComponent(glm::vec3 Position_, glm::vec3 Rotation_, glm::vec3 Scale_);
	~TransfromComponent();

	void UpdateTransformComponent(glm::vec3 Position_, glm::vec3 Rotation_, glm::vec3 Scale_);
	glm::vec3 GetPosition();
	glm::vec3 GetRotation();
	glm::vec3 GetScale();
public:
	glm::vec3 Position = glm::vec3(0.f);
	glm::vec3 Rotation = glm::vec3(0.f);
	glm::vec3 Scale = glm::vec3(0.f);

	template <class Archive>
	void serialize(Archive& ar)
	{
		ar(
			CEREAL_NVP(Position),
			CEREAL_NVP(Rotation),
			CEREAL_NVP(Scale)
		);
	}
};
