#pragma once

#ifdef ENGINE_EXPORTS
#define ENGINE_API __declspec(dllexport)
#else
#define ENGINE_API __declspec(dllimport)
#endif


#include <glad/glad.h>
#include <GLFW/glfw3.h>

class ENGINE_API Time
{
public:
	static float timeStarted;
	static float getTime();
};

