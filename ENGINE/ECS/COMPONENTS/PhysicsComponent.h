#pragma once

#ifdef ENGINE_EXPORTS
#define ENGINE_API __declspec(dllexport)
#else
#define ENGINE_API __declspec(dllimport)
#endif

#include <reactphysics3d/ReactPhysics3D.h>


enum class PHYSICSCOLLOIDERTYPE
{
	BOXCOLLOIDER = 0,
	SPHERECOLLOIDER = 1
};

enum class PHYSICSBODYTYPE
{
	STATICMESH = rp3d::BodyType::STATIC,
	DYMAMICMESH = rp3d::BodyType::DYNAMIC,
	KINEMATICMESH = rp3d::BodyType::KINEMATIC
};

struct ENGINE_API PhysicsComponent
{
public:
	PhysicsComponent() = default;
	PhysicsComponent(
		PHYSICSCOLLOIDERTYPE colloiderType, 
		PHYSICSBODYTYPE bodyType,
		bool IsDebugEnabled
	);
	~PhysicsComponent();

	rp3d::RigidBody* rigidBody;
	PHYSICSCOLLOIDERTYPE colloiderType;
	PHYSICSBODYTYPE bodyType;
	bool IsDebugEnabled;
};

