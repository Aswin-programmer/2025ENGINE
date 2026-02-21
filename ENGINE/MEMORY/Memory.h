#pragma once

#ifdef ENGINE_EXPORTS
#define ENGINE_API __declspec(dllexport)
#else
#define ENGINE_API __declspec(dllimport)
#endif

#include <iostream>
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>
#include <psapi.h>

class ENGINE_API Memory {
public:
    Memory();
    ~Memory();
    void PrintCurrentMemoryConsumption();

private:
    PROCESS_MEMORY_COUNTERS pmc;
    HANDLE currentProcess;

    double currentRam;  // WorkingSetSize
    double peekRam;     // PeekWorkingSetSize
    double virtualSize; // PageFileUsage
};
