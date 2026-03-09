#include "PhysicsComponent.h"

PhysicsComponent::PhysicsComponent(
	PHYSICSCOLLOIDERTYPE colloiderType,
	PHYSICSBODYTYPE bodyType,
	bool IsDebugEnabled
)
	:
	rigidBody{ nullptr },
	colloider{nullptr},
	colloiderType{colloiderType},
	bodyType{bodyType},
	IsDebugEnabled{IsDebugEnabled}
{
}


PhysicsComponent::~PhysicsComponent()
{

}

