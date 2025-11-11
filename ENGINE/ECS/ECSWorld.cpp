#include "ECSWorld.h"

ECSWorld::ECSWorld()
	:
	ecsWorld{std::make_shared<flecs::world>()}
{

}

ECSWorld::~ECSWorld()
{
}

void ECSWorld::InitECSWorld()
{
	ecsWorld->component<TransfromComponent>();
	ecsWorld->component<MeshComponent>();
}

flecs::entity ECSWorld::CreateEntity(const std::string& name)
{
	flecs::entity entity = ecsWorld->entity(name.c_str());
	return entity;
}

void ECSWorld::DestroyEntity(flecs::entity entity)
{
	if (entity.is_alive())
	{
		entity.destruct();
	}
}

flecs::entity ECSWorld::FindEntityByName(const std::string& name)
{
	flecs::entity found = ecsWorld->lookup(name.c_str());
	if (found.is_alive())
	{
		return found;
	}
	return flecs::entity::null();
}
