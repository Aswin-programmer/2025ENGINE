#include "Keyboard.h"

std::unordered_map<int, Button> Keyboard::s_keys;

void Keyboard::Init()
{
    // List of all supported keys
    const int allKeys[] =
    {
        KEY_BACKSPACE, KEY_TAB, KEY_RETURN, KEY_PAUSE, KEY_ESCAPE, KEY_SPACE,
        KEY_COMMA, KEY_MINUS, KEY_PERIOD, KEY_SLASH,
        KEY_0, KEY_1, KEY_2, KEY_3, KEY_4,
        KEY_5, KEY_6, KEY_7, KEY_8, KEY_9,

        KEY_SEMICOLON, KEY_EQUALS, KEY_LEFTBRACKET,
        KEY_BACKSLASH, KEY_RIGHTBRACKET,

        KEY_A, KEY_B, KEY_C, KEY_D, KEY_E,
        KEY_F, KEY_G, KEY_H, KEY_I, KEY_J,
        KEY_K, KEY_L, KEY_M, KEY_N, KEY_O,
        KEY_P, KEY_Q, KEY_R, KEY_S, KEY_T,
        KEY_U, KEY_V, KEY_W, KEY_X, KEY_Y, KEY_Z,

        KEY_DELETE, KEY_CAPSLOCK,

        KEY_F1, KEY_F2, KEY_F3, KEY_F4, KEY_F5,
        KEY_F6, KEY_F7, KEY_F8, KEY_F9, KEY_F10,
        KEY_F11, KEY_F12,

        KEY_INSERT, KEY_HOME, KEY_PAGEUP, KEY_END,
        KEY_PAGEDOWN, KEY_RIGHT, KEY_LEFT, KEY_DOWN, KEY_UP,

        KEY_NUMLOCK, KEY_KP_DIVIDE, KEY_KP_MULTIPLY, KEY_KP_MINUS,
        KEY_KP_PLUS, KEY_KP_ENTER, KEY_KP1, KEY_KP2, KEY_KP3,
        KEY_KP4, KEY_KP5, KEY_KP6, KEY_KP7, KEY_KP8, KEY_KP9,
        KEY_KP0, KEY_KP_PERIOD,

        KEY_LCTRL, KEY_LSHIFT, KEY_LALT,
        KEY_RCTRL, KEY_RSHIFT, KEY_RALT
    };

    for (int key : allKeys)
        s_keys.emplace(key, Button{});
}

void Keyboard::Update()
{
    for (auto& [key, btn] : s_keys)
        btn.Reset();
}

void Keyboard::OnKeyPressed(int key)
{
    auto it = s_keys.find(key);
    if (it == s_keys.end()) return;

    it->second.Update(true);
}

void Keyboard::OnKeyReleased(int key)
{
    auto it = s_keys.find(key);
    if (it == s_keys.end()) return;

    it->second.Update(false);
}

bool Keyboard::IsKeyPressed(int key)
{
    auto it = s_keys.find(key);
    return (it != s_keys.end()) ? it->second.bIsPressed : false;
}

bool Keyboard::IsKeyJustPressed(int key)
{
    auto it = s_keys.find(key);
    return (it != s_keys.end()) ? it->second.bJustPressed : false;
}

bool Keyboard::IsKeyJustReleased(int key)
{
    auto it = s_keys.find(key);
    return (it != s_keys.end()) ? it->second.bJustReleased : false;
}
