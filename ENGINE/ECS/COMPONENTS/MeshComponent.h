#pragma once

#ifdef ENGINE_EXPORTS
#define ENGINE_API __declspec(dllexport)
#else
#define ENGINE_API __declspec(dllimport)
#endif

#include <string>

#include <glm/glm.hpp>

struct ENGINE_API MeshComponent
{
public:
	MeshComponent() = default;
	MeshComponent(std::string MeshName_);
	~MeshComponent() = default;

	void UpdateMeshName(std::string MeshName_);
	std::string GetMeshName();
public:
	std::string MeshName = "NIL";
};
