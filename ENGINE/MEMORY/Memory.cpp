#include "Memory.h"
#include <processthreadsapi.h>

Memory::Memory() : currentProcess(GetCurrentProcess()), currentRam{0}, peekRam{0}, virtualSize{0} {}

Memory::~Memory() {}

void Memory::PrintCurrentMemoryConsumption() {
    if (GetProcessMemoryInfo(currentProcess, &pmc, sizeof(pmc))) {
        const double MEGABYTE = 1024.0 * 1024.0;
        std::cout << "\n -- MEMORY INFORMATION --" << std::endl;
        currentRam = static_cast<double>(pmc.WorkingSetSize / MEGABYTE);
        std::cout << "Current Ram (Working Set) : " << currentRam << "MB" << std::endl;
    }
}
