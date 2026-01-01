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
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

#include "ReactPhysics3DCore.h"
#include "../ECS/COMPONENTS/PhysicsComponent.h"
#include "../ECS/COMPONENTS/TransfromComponent.h"
#include "../ECS/COMPONENTS/MeshComponent.h"
#include "ReactPhysics3DLoader.h"
#include "../GlobalInformation/GlobalInformation.h"

class ENGINE_API ReactPhysics3DSystem
{
public:
	ReactPhysics3DSystem();
	ReactPhysics3DSystem(
		std::shared_ptr<ReactPhysics3DCore> reactPhysics3DCore,  
		std::shared_ptr<flecs::world> ecsWorld
	);
	~ReactPhysics3DSystem();

	void CommitReactPhysics3DState();
	void UpdateReactPhysics3DSystem();
private:
	std::shared_ptr<ReactPhysics3DCore> reactPhysics3DCore;
	std::shared_ptr<flecs::world> ecsWorld;
	flecs::query<TransfromComponent, MeshComponent, PhysicsComponent> meshPhysicsQuery;
	static bool IsDebugEnabled;
};

