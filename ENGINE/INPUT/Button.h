#pragma once

#ifdef ENGINE_EXPORTS
#define ENGINE_API __declspec(dllexport)
#else
#define ENGINE_API __declspec(dllimport)
#endif


struct ENGINE_API Button
{
	bool bIsPressed{ false }, bJustPressed{ false }, bJustReleased{ false };
	void Update(bool bPressed)
	{
		bJustPressed = !bIsPressed && bPressed;
		bJustReleased = bIsPressed && !bPressed;
		bIsPressed = bPressed;
	}
	void Reset()
	{
		bJustPressed = false;
		bJustReleased = false;
	}
};


