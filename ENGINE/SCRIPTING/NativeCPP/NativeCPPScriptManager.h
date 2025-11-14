#pragma once

#ifdef ENGINE_EXPORTS
#define ENGINE_API __declspec(dllexport)
#else
#define ENGINE_API __declspec(dllimport)
#endif

#ifdef _WIN32
#define NOMINMAX
	#include <Windows.h>
	using LibHandle = HMODULE;
#else
	// It will be implemented later.
#endif // _WIN32

#include "NativeCPPGlobalScript.h"

#include <iostream>
#include <filesystem>
#include <thread>

#include "../../ECS/ECSWorld.h"

class ENGINE_API NativeCPPScriptManager
{
public:
	NativeCPPScriptManager();
	NativeCPPScriptManager(std::string dllBuildPath_);
	~NativeCPPScriptManager();

	void PrintLastError(const std::string& prefix);

	bool LoadDLL();
	bool ReloadIfUpdated();
	bool UnloadDLL();

	void UpdateScript();

	void LoadDependencies(std::shared_ptr<ECSWorld> ecsWorld_);


private:
	std::string dllBuildPath;
	LibHandle dllHandle = nullptr;
	CreateScriptFunc createScriptFunction = nullptr;
	DestroyScriptFunc DestroyScriptFunction = nullptr;
	std::filesystem::file_time_type lastWrite;
	std::unique_ptr<NativeCPPGlobalScript> scriptInstance;

	LibHandle LoadLibraryFile(const std::string& path);
	void UnloadLibraryFile(LibHandle h);
	void* GetSymbol(LibHandle h, const char* name);
	std::string MakeTempCopy(const std::string& srcPath);

	// Dependencies to be passed
	std::shared_ptr<ECSWorld> ecsWorld;

};
