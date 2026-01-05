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

namespace cereal
{
	template <class Archive>
	void serialize(Archive& ar, glm::vec3& v)
	{
		ar(
			CEREAL_NVP(v.x),
			CEREAL_NVP(v.y),
			CEREAL_NVP(v.z)
		);
	}
}

