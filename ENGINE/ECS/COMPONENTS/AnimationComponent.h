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

#include "../../WINDOW/Window.h" // If I remove then its compliing

struct ENGINE_API AnimationComponent
{
public:
	AnimationComponent() = default;
	AnimationComponent(bool isAnimationEnabled_);

	double GetCurrentOnTime();

    template <class Archive>
    void serialize(Archive& ar)
    {
        ar(
            CEREAL_NVP(isAnimationEnabled),
            CEREAL_NVP(currentTime)
        );
    }
private:
	bool isAnimationEnabled = false;
	double currentTime;
};