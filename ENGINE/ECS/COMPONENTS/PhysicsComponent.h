#pragma once

#ifdef ENGINE_EXPORTS
#define ENGINE_API __declspec(dllexport)
#else
#define ENGINE_API __declspec(dllimport)
#endif

#include <reactphysics3d/ReactPhysics3D.h>
#include <cereal/cereal.hpp>
#include <cereal/types/string.hpp>     // std::string
#include <cereal/types/vector.hpp>     // std::vector
#include <cereal/types/optional.hpp>


enum class PHYSICSCOLLOIDERTYPE
{
	BOXCOLLOIDER = 0,
	SPHERECOLLOIDER = 1
};

enum class PHYSICSBODYTYPE
{
	STATICMESH = static_cast<int>(rp3d::BodyType::STATIC),
	DYMAMICMESH = static_cast<int>(rp3d::BodyType::DYNAMIC),
	KINEMATICMESH = static_cast<int>(rp3d::BodyType::KINEMATIC)
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
	rp3d::Collider* colloider;
	PHYSICSCOLLOIDERTYPE colloiderType;
	PHYSICSBODYTYPE bodyType;
	bool IsDebugEnabled;

	template <class Archive>
	void serialize(Archive& ar)
	{
		ar(
			CEREAL_NVP(colloiderType),
			CEREAL_NVP(bodyType),
			CEREAL_NVP(IsDebugEnabled)
		);
	}
};

