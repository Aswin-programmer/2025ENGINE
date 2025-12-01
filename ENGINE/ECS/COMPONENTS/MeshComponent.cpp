#include "MeshComponent.h"

MeshComponent::MeshComponent(
	std::string MeshName_,
	float ambientStrength,
	float diffuseStrength,
	float specularStrength
)
	:
	MeshName{MeshName_},
	ambientStrength{ ambientStrength },
	diffuseStrength{ diffuseStrength },
	specularStrength{ specularStrength }
{
}

void MeshComponent::UpdateMeshName(std::string MeshName_)
{
	MeshName = MeshName_;
}

std::string MeshComponent::GetMeshName()
{
	return MeshName;
}

std::string MeshComponent::GetStrippedMeshName()
{
	size_t dot = MeshName.find_last_of('.');
	std::string nameOnly = MeshName.substr(0, dot);
	return nameOnly;
}
