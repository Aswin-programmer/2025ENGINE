#pragma once

#include <iostream>
#pragma once

#ifdef ENGINE_EXPORTS
#define ENGINE_API __declspec(dllexport)
#else
#define ENGINE_API __declspec(dllimport)
#endif


#include <vector>

#include "../../ECS/ECSWorld.h"

class ENGINE_API NativeCPPGlobalScript
{
public:
	virtual ~NativeCPPGlobalScript() = default;

	void SetupDependencies(std::shared_ptr<ECSWorld> ecsWorld_) { ecsWorld = ecsWorld_; }

	// Lifecycle
	virtual void OnIntialize() {}
	virtual void OnUpdate() {}
	virtual void OnShutDown() {}


public:
	std::shared_ptr<ECSWorld> ecsWorld;
	std::vector<int> v1 = std::vector<int>(5, -1);
};

using CreateScriptFunc = NativeCPPGlobalScript * (*)();
using DestroyScriptFunc = void (*)(NativeCPPGlobalScript*);
