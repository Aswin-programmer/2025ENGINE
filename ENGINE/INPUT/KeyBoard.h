#pragma once

#ifdef ENGINE_EXPORTS
#define ENGINE_API __declspec(dllexport)
#else
#define ENGINE_API __declspec(dllimport)
#endif

#include <unordered_map>
#include <iostream>

#include "Button.h"
#include "Keys.h"

class ENGINE_API Keyboard
{
private:
    static std::unordered_map<int, Button> s_keys;

public:
    static void Init();
    static void Update();

    static void OnKeyPressed(int key);
    static void OnKeyReleased(int key);

    static bool IsKeyPressed(int key);
    static bool IsKeyJustPressed(int key);
    static bool IsKeyJustReleased(int key);
};
