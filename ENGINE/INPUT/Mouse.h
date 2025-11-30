#pragma once

#ifdef ENGINE_EXPORTS
#define ENGINE_API __declspec(dllexport)
#else
#define ENGINE_API __declspec(dllimport)
#endif

#include <unordered_map>
#include <tuple>
#include <GLFW/glfw3.h>

#include "Button.h"
#include "MouseButtons.h"

class ENGINE_API Mouse
{
private:
    static std::unordered_map<int, Button> s_buttons;

    static double s_X;
    static double s_Y;
    static double s_WheelX;
    static double s_WheelY;
    static bool   s_Moving;

public:
    static void Init();           // initialize all buttons
    static void Update();

    static void OnBtnPressed(int btn);
    static void OnBtnReleased(int btn);

    static bool IsBtnPressed(int btn);
    static bool IsBtnJustPressed(int btn);
    static bool IsBtnJustReleased(int btn);

    static std::tuple<int, int> GetMouseScreenPosition(GLFWwindow* window);

    static inline void SetMouseWheelX(double val) { s_WheelX = val; }
    static inline void SetMouseWheelY(double val) { s_WheelY = val; }
    static inline void SetMouseMoving(bool moving) { s_Moving = moving; }

    static inline double GetMouseWheelX() { return s_WheelX; }
    static inline double GetMouseWheelY() { return s_WheelY; }
    static inline bool   IsMouseMoving() { return s_Moving; }
};
