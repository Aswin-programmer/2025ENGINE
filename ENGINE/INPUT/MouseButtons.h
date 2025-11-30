#pragma once

#include <GLFW/glfw3.h>

#ifdef ENGINE_EXPORTS
#define ENGINE_API __declspec(dllexport)
#else
#define ENGINE_API __declspec(dllimport)
#endif


extern ENGINE_API const int MOUSE_UNKNOWN;
extern ENGINE_API const int MOUSE_LEFT;
extern ENGINE_API const int MOUSE_MIDDLE;
extern ENGINE_API const int MOUSE_RIGHT;
