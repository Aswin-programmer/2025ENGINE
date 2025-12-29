#pragma once

#ifdef ENGINE_EXPORTS
#define ENGINE_API __declspec(dllexport)
#else
#define ENGINE_API __declspec(dllimport)
#endif

#include <iostream>
#include <memory>

#include <reactphysics3d/ReactPhysics3D.h>

class ENGINE_API ReactPhysics3DCore
{
public:
	ReactPhysics3DCore();
	~ReactPhysics3DCore();

	void InitReactPhysics3DCore();

	rp3d::PhysicsWorld* GetPhysicsWorld() { return physicsWorld; }

	bool IsDebugEnabled();

	void EnableDebug();

private:
	bool IsInitialized;
	std::shared_ptr<rp3d::PhysicsCommon> physicsCommon;
	rp3d::PhysicsWorld* physicsWorld;
};