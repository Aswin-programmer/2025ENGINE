#include "ReactPhysics3DCore.h"

ReactPhysics3DCore::ReactPhysics3DCore()
	:
	physicsCommon{std::make_shared<rp3d::PhysicsCommon>()},
	IsInitialized{false}
{
}

ReactPhysics3DCore::~ReactPhysics3DCore()
{
}

void ReactPhysics3DCore::InitReactPhysics3DCore()
{
	assert(!IsInitialized && "ReactPhysics3DCore already initialized!");
	physicsWorld = physicsCommon->createPhysicsWorld();
	IsInitialized = true;
}

bool ReactPhysics3DCore::IsDebugEnabled()
{
	if (!IsInitialized)
	{
		assert(IsInitialized && "The ReactPhysics3DCore Is not Initialized!.");
		return false;
	}

	return physicsWorld->getIsDebugRenderingEnabled();
}

void ReactPhysics3DCore::EnableDebug()
{
	if (!IsInitialized)
	{
		assert(!IsInitialized && "The ReactPhysics3DCore Is not Initialized!.");
		return;
	}

	physicsWorld->setIsDebugRenderingEnabled(true);
	rp3d::DebugRenderer& dbg = physicsWorld->getDebugRenderer();

	dbg.setIsDebugItemDisplayed(rp3d::DebugRenderer::DebugItem::COLLISION_SHAPE, true);
	dbg.setIsDebugItemDisplayed(rp3d::DebugRenderer::DebugItem::COLLIDER_AABB, true);
	dbg.setIsDebugItemDisplayed(rp3d::DebugRenderer::DebugItem::CONTACT_POINT, true);
}

void ReactPhysics3DCore::DisableDebug()
{
	if (!IsInitialized)
	{
		assert(!IsInitialized && "The ReactPhysics3DCore Is not Initialized!.");
		return;
	}

	physicsWorld->setIsDebugRenderingEnabled(false);
}

void ReactPhysics3DCore::SetDebug(bool debug)
{
	if (!IsInitialized)
	{
		assert(!IsInitialized && "The ReactPhysics3DCore Is not Initialized!.");
		return;
	}

	physicsWorld->setIsDebugRenderingEnabled(debug);
	rp3d::DebugRenderer& dbg = physicsWorld->getDebugRenderer();

	dbg.setIsDebugItemDisplayed(rp3d::DebugRenderer::DebugItem::COLLISION_SHAPE, true);
	dbg.setIsDebugItemDisplayed(rp3d::DebugRenderer::DebugItem::COLLIDER_AABB, true);
	dbg.setIsDebugItemDisplayed(rp3d::DebugRenderer::DebugItem::CONTACT_POINT, true);
}
