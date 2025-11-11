#pragma once

#ifdef ENGINE_EXPORTS
#define ENGINE_API __declspec(dllexport)
#else
#define ENGINE_API __declspec(dllimport)
#endif

#include <iostream>

#include <flecs.h>

struct Transfrom1
{
    Transfrom1() = default;             // <-- add this
    Transfrom1(int x, int y)
        : x{ x }, y{ y }
    {
    }

    int x = 0;
    int y = 0;
};

struct Transfrom2
{
    Transfrom2() = default;             // <-- add this too
    Transfrom2(int x, int y)
        : x{ x }, y{ y }
    {
    }

    int x = 0;
    int y = 0;
};



class ENGINE_API FlecsTest
{
public:
	FlecsTest();
	~FlecsTest();

	void Test();

private:
	flecs::world world;
};
