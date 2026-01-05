#pragma once

#ifdef ENGINE_EXPORTS
#define ENGINE_API __declspec(dllexport)
#else
#define ENGINE_API __declspec(dllimport)
#endif

#include <string>

#include <glm/glm.hpp>
#include <cereal/cereal.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/optional.hpp>

struct ENGINE_API MeshComponent
{
public:
	MeshComponent() = default;
	MeshComponent(
		std::string MeshName_,
		float ambientStrength,
		float diffuseStrength,
		float specularStrength
	);
	~MeshComponent() = default;

	void UpdateMeshName(std::string MeshName_);
	std::string GetMeshName();
	std::string GetStrippedMeshName();
public:
	std::string MeshName = "NIL";
	float ambientStrength;
	float diffuseStrength;
	float specularStrength;

	template <class Archive>
	void serialize(Archive& ar)
	{
		ar(
			CEREAL_NVP(MeshName),
			CEREAL_NVP(ambientStrength),
			CEREAL_NVP(diffuseStrength),
			CEREAL_NVP(specularStrength)
		);
	}
};
