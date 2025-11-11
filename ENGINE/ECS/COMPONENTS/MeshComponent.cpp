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
