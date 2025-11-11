#pragma once

#ifdef ENGINE_EXPORTS
#define ENGINE_API __declspec(dllexport)
#else
#define ENGINE_API __declspec(dllimport)
#endif

#include <string>
#include <memory>

#include <flecs.h>

#include "COMPONENTS/TransfromComponent.h"
#include "COMPONENTS/MeshComponent.h"

class ENGINE_API ECSWorld
{
public:
	ECSWorld();
	~ECSWorld();

	void InitECSWorld(); // Used for registering the components and etc.

	// Entity management
	flecs::entity CreateEntity(const std::string& name = "");
	void DestroyEntity(flecs::entity entity);
	flecs::entity FindEntityByName(const std::string& name);

	std::shared_ptr<flecs::world> GetWorld() { return ecsWorld; }

private:
	std::shared_ptr<flecs::world> ecsWorld;
};