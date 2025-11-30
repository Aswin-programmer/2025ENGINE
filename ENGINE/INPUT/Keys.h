#pragma once

#include <GLFW/glfw3.h>

#ifdef ENGINE_EXPORTS
#define ENGINE_API __declspec(dllexport)
#else
#define ENGINE_API __declspec(dllimport)
#endif

// Declare everything as extern so the DLL exports the variables.
// Definitions are in Keys.cpp.

extern ENGINE_API const int KEY_UNKNOWN;
extern ENGINE_API const int KEY_BACKSPACE;
extern ENGINE_API const int KEY_TAB;
extern ENGINE_API const int KEY_RETURN;
extern ENGINE_API const int KEY_PAUSE;
extern ENGINE_API const int KEY_ESCAPE;
extern ENGINE_API const int KEY_SPACE;
extern ENGINE_API const int KEY_COMMA;
extern ENGINE_API const int KEY_MINUS;
extern ENGINE_API const int KEY_PERIOD;
extern ENGINE_API const int KEY_SLASH;
extern ENGINE_API const int KEY_0;
extern ENGINE_API const int KEY_1;
extern ENGINE_API const int KEY_2;
extern ENGINE_API const int KEY_3;
extern ENGINE_API const int KEY_4;
extern ENGINE_API const int KEY_5;
extern ENGINE_API const int KEY_6;
extern ENGINE_API const int KEY_7;
extern ENGINE_API const int KEY_8;
extern ENGINE_API const int KEY_9;
extern ENGINE_API const int KEY_SEMICOLON;
extern ENGINE_API const int KEY_EQUALS;
extern ENGINE_API const int KEY_LEFTBRACKET;
extern ENGINE_API const int KEY_BACKSLASH;
extern ENGINE_API const int KEY_RIGHTBRACKET;

extern ENGINE_API const int KEY_A;
extern ENGINE_API const int KEY_B;
extern ENGINE_API const int KEY_C;
extern ENGINE_API const int KEY_D;
extern ENGINE_API const int KEY_E;
extern ENGINE_API const int KEY_F;
extern ENGINE_API const int KEY_G;
extern ENGINE_API const int KEY_H;
extern ENGINE_API const int KEY_I;
extern ENGINE_API const int KEY_J;
extern ENGINE_API const int KEY_K;
extern ENGINE_API const int KEY_L;
extern ENGINE_API const int KEY_M;
extern ENGINE_API const int KEY_N;
extern ENGINE_API const int KEY_O;
extern ENGINE_API const int KEY_P;
extern ENGINE_API const int KEY_Q;
extern ENGINE_API const int KEY_R;
extern ENGINE_API const int KEY_S;
extern ENGINE_API const int KEY_T;
extern ENGINE_API const int KEY_U;
extern ENGINE_API const int KEY_V;
extern ENGINE_API const int KEY_W;
extern ENGINE_API const int KEY_X;
extern ENGINE_API const int KEY_Y;
extern ENGINE_API const int KEY_Z;

extern ENGINE_API const int KEY_DELETE;
extern ENGINE_API const int KEY_CAPSLOCK;

extern ENGINE_API const int KEY_F1;
extern ENGINE_API const int KEY_F2;
extern ENGINE_API const int KEY_F3;
extern ENGINE_API const int KEY_F4;
extern ENGINE_API const int KEY_F5;
extern ENGINE_API const int KEY_F6;
extern ENGINE_API const int KEY_F7;
extern ENGINE_API const int KEY_F8;
extern ENGINE_API const int KEY_F9;
extern ENGINE_API const int KEY_F10;
extern ENGINE_API const int KEY_F11;
extern ENGINE_API const int KEY_F12;

extern ENGINE_API const int KEY_INSERT;
extern ENGINE_API const int KEY_HOME;
extern ENGINE_API const int KEY_PAGEUP;
extern ENGINE_API const int KEY_END;
extern ENGINE_API const int KEY_PAGEDOWN;
extern ENGINE_API const int KEY_RIGHT;
extern ENGINE_API const int KEY_LEFT;
extern ENGINE_API const int KEY_DOWN;
extern ENGINE_API const int KEY_UP;

extern ENGINE_API const int KEY_NUMLOCK;
extern ENGINE_API const int KEY_KP_DIVIDE;
extern ENGINE_API const int KEY_KP_MULTIPLY;
extern ENGINE_API const int KEY_KP_MINUS;
extern ENGINE_API const int KEY_KP_PLUS;
extern ENGINE_API const int KEY_KP_ENTER;
extern ENGINE_API const int KEY_KP1;
extern ENGINE_API const int KEY_KP2;
extern ENGINE_API const int KEY_KP3;
extern ENGINE_API const int KEY_KP4;
extern ENGINE_API const int KEY_KP5;
extern ENGINE_API const int KEY_KP6;
extern ENGINE_API const int KEY_KP7;
extern ENGINE_API const int KEY_KP8;
extern ENGINE_API const int KEY_KP9;
extern ENGINE_API const int KEY_KP0;
extern ENGINE_API const int KEY_KP_PERIOD;

extern ENGINE_API const int KEY_LCTRL;
extern ENGINE_API const int KEY_LSHIFT;
extern ENGINE_API const int KEY_LALT;
extern ENGINE_API const int KEY_RCTRL;
extern ENGINE_API const int KEY_RSHIFT;
extern ENGINE_API const int KEY_RALT;
