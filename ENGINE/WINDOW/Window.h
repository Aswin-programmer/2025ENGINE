#pragma once

#ifdef ENGINE_EXPORTS
#define ENGINE_API __declspec(dllexport)
#else
#define ENGINE_API __declspec(dllimport)
#endif

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <memory>

#include "../UTILITIES/Time.h"
#include "PROFILING/Profiler.h"

class ENGINE_API Window {
public:
    static bool init(const char* title);
    static void processInput();
    static bool shouldClose();
    static void update();
    static void clearScreen();
    static GLFWwindow* getGLFWWindow();

    static float getdt() { return dt; }
    static void getFPS();
    static float GetFPSValue();
    static int getWidth() { return m_width; }
    static int getHeight() { return m_height; }

    static void cleanup();

private:
    static unsigned int m_width;
    static unsigned int m_height;
    static const char* m_title;
    static GLFWwindow* m_window;

    static float beginTime;
    static float endTime;
    static float dt;

    static void framebufferSizeCallback(GLFWwindow* window, int width, int height);
};

// ? Declare exported static color variables outside the class
ENGINE_API extern float Window_r;
ENGINE_API extern float Window_g;
ENGINE_API extern float Window_b;
ENGINE_API extern float Window_a;
