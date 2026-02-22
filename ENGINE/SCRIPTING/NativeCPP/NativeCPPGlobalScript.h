#pragma once
#ifdef ENGINE_EXPORTS
#define ENGINE_API __declspec(dllexport)
#else
#define ENGINE_API __declspec(dllimport)
#endif

#include <iostream>
#include <vector>

#include <OliUI.h>

#include "../../ECS/ECSWorld.h"
#include "../../REACTPHYSICS3D/ReactPhysics3DCore.h"

class ENGINE_API NativeCPPGlobalScript
{
public:
	virtual ~NativeCPPGlobalScript() = default;

	void SetupDependencies(
		std::shared_ptr<ECSWorld> ecsWorld_, 
		OliUIMainContext* oliUIMainContext_,
		std::shared_ptr<ReactPhysics3DCore> physicsCore_
	) 
	{ 
		ecsWorld = ecsWorld_; 
		oliUIMainContext = oliUIMainContext_;
		physicsCore = physicsCore_;
	}

	// Lifecycle
	virtual void OnIntialize() {}
	virtual void OnUpdate() {}
	virtual void OnShutDown() {}


public:
	std::shared_ptr<ECSWorld> ecsWorld;
	OliUIMainContext* oliUIMainContext;
	std::shared_ptr<ReactPhysics3DCore> physicsCore;
	std::vector<int> v1 = std::vector<int>(5, -1);
};

using CreateScriptFunc = NativeCPPGlobalScript * (*)();
using DestroyScriptFunc = void (*)(NativeCPPGlobalScript*);
