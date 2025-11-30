#include "Mouse.h"

std::unordered_map<int, Button> Mouse::s_buttons = {
    { MOUSE_LEFT, Button{} },
    { MOUSE_MIDDLE, Button{} },
    { MOUSE_RIGHT, Button{} },
};

double Mouse::s_X = 0;
double Mouse::s_Y = 0;
double Mouse::s_WheelX = 0;
double Mouse::s_WheelY = 0;
bool   Mouse::s_Moving = false;

void Mouse::Init()
{
    // Reset all buttons
    for (auto& [key, button] : s_buttons)
        button = Button{};
}

void Mouse::Update()
{
    for (auto& [btn, button] : s_buttons)
        button.Reset();

    s_WheelX = 0;
    s_WheelY = 0;
    s_Moving = false;
}

void Mouse::OnBtnPressed(int btn)
{
    auto it = s_buttons.find(btn);
    if (it == s_buttons.end()) return;

    it->second.Update(true);
}

void Mouse::OnBtnReleased(int btn)
{
    auto it = s_buttons.find(btn);
    if (it == s_buttons.end()) return;

    it->second.Update(false);
}

bool Mouse::IsBtnPressed(int btn)
{
    auto it = s_buttons.find(btn);
    if (it == s_buttons.end()) return false;

    return it->second.bIsPressed;
}

bool Mouse::IsBtnJustPressed(int btn)
{
    auto it = s_buttons.find(btn);
    if (it == s_buttons.end()) return false;

    return it->second.bJustPressed;
}

bool Mouse::IsBtnJustReleased(int btn)
{
    auto it = s_buttons.find(btn);
    if (it == s_buttons.end()) return false;

    return it->second.bJustReleased;
}

std::tuple<int, int> Mouse::GetMouseScreenPosition(GLFWwindow* window)
{
    glfwGetCursorPos(window, &s_X, &s_Y);
    return std::make_tuple((int)s_X, (int)s_Y);
}
