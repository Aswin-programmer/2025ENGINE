#include "TransfromComponent.h"

TransfromComponent::TransfromComponent(glm::vec3 Position_, glm::vec3 Rotation_, glm::vec3 Scale_)
	:
	Position{Position_},
	Rotation{Rotation_},
	Scale{Scale_}
{
}

TransfromComponent::~TransfromComponent()
{
}

void TransfromComponent::UpdateTransformComponent(glm::vec3 Position_
	, glm::vec3 Rotation_, glm::vec3 Scale_)
{
	Position = Position_;
	Rotation = Rotation_;
	Scale = Scale_;
}

glm::vec3 TransfromComponent::GetPosition()
{
	return Position;
}

glm::vec3 TransfromComponent::GetRotation()
{
	return Rotation;
}

glm::vec3 TransfromComponent::GetScale()
{
	return Scale;
}
