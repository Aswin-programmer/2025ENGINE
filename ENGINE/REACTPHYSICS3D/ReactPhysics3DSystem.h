#pragma once

#ifdef ENGINE_EXPORTS
#define ENGINE_API __declspec(dllexport)
#else
#define ENGINE_API __declspec(dllimport)
#endif

#include <iostream>
#include <memory>

#include <flecs.h>
#include <reactphysics3d/ReactPhysics3D.h>

#include "ReactPhysics3DCore.h"

class ENGINE_API ReactPhysics3DSystem
{
public:
	ReactPhysics3DSystem();
	ReactPhysics3DSystem(
		std::shared_ptr<ReactPhysics3DCore> reactPhysics3DCore,  
		std::shared_ptr<flecs::world> ecsWorld_
	);
	~ReactPhysics3DSystem();

	void UpdateReactPhysics3DSystem();
private:

};

