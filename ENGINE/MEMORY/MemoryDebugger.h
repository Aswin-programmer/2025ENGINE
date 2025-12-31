#pragma once

#include <windows.h>
#include <psapi.h>
#include <iostream>

double get_current_ram_usage() {
    PROCESS_MEMORY_COUNTERS pmc;
    if (GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc))) {
        return (double)pmc.WorkingSetSize / (1024.0 * 1024.0);
    }
    return 0.0;
}

void print_memory_usage() {
	// 1. Declare the structure to hold memory information
	PROCESS_MEMORY_COUNTERS pmc;

	// 2. Get the handle for the current process
	HANDLE hProcess = GetCurrentProcess();

	// 3. Call the function
	if (GetProcessMemoryInfo(hProcess, &pmc, sizeof(pmc))) {
		// Define conversion factor (bytes to Megabytes)
		const double MEGABYTE = 1024.0 * 1024.0;

		std::cout << "\n--- Memory Information ---" << std::endl;

		// WorkingSetSize is the current amount of physical memory (RAM) used.
		double current_ram = (double)pmc.WorkingSetSize / MEGABYTE;
		std::cout << "Current RAM (Working Set): "
			<< current_ram << " MB" << std::endl;

		// PeakWorkingSetSize is the peak physical memory (Max RSS) used.
		double peak_ram = (double)pmc.PeakWorkingSetSize / MEGABYTE;
		std::cout << "Peak RAM (Max RSS):        "
			<< peak_ram << " MB" << std::endl;

		// PagefileUsage is the total size of Virtual Memory committed by the process.
		double virtual_size = (double)pmc.PagefileUsage / MEGABYTE;
		std::cout << "Current Virtual Size:      "
			<< virtual_size << " MB" << std::endl;

	}
	else {
		std::cerr << "Failed to get process memory info." << std::endl;
	}
}

