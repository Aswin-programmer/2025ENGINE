#pragma once

#ifdef ENGINE_EXPORTS
#define ENGINE_API __declspec(dllexport)
#else
#define ENGINE_API __declspec(dllimport)
#endif

#include <glm/glm.hpp>

#include "../../WINDOW/Window.h"

struct ENGINE_API AnimationComponent
{
public:
	AnimationComponent() = default;
	AnimationComponent(bool isAnimationEnabled_);

	double GetCurrentOnTime();
private:
	bool isAnimationEnabled = false;
	double currentTime;
};