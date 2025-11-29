#include "MeshComponent.h"

MeshComponent::MeshComponent(std::string MeshName_)
	:
	MeshName{MeshName_}
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
