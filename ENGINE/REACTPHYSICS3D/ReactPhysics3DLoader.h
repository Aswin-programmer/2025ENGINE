#pragma once

#ifdef ENGINE_EXPORTS
#define ENGINE_API __declspec(dllexport)
#else
#define ENGINE_API __declspec(dllimport)
#endif

#include <string>
#include <unordered_map>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <tiny_gltf.h>

struct ENGINE_API ReactPhysics3DBoxColloider
{
	ReactPhysics3DBoxColloider(glm::vec3 positionOffset, glm::quat rotation, glm::vec3 halfExtents)
		:
		positionOffset{positionOffset},
		rotation{rotation},
		halfExtents{halfExtents}
	{
	}
	glm::vec3 positionOffset;
	glm::quat rotation;
	glm::vec3 halfExtents;
};


class ENGINE_API ReactPhysics3DLoader
{
public:
	ReactPhysics3DLoader(const ReactPhysics3DLoader&) = delete;
	ReactPhysics3DLoader& operator=(const ReactPhysics3DLoader&) = delete;

	static bool LoadReactPhysics3DBoxColloiderFromGLTFModel(std::string& fileName, tinygltf::Model& model);
	static ReactPhysics3DBoxColloider& GetReactPhysics3DBoxColloiderFromGLTFModel(std::string& fileName);
	static void DeleteAllReactPhysics3DColloiders();
private:
	static std::unordered_map<std::string, ReactPhysics3DBoxColloider> reactPhysics3dBoxColloiderContainer;
};

