#pragma once

#ifdef ENGINE_EXPORTS
#define ENGINE_API __declspec(dllexport)
#else
#define ENGINE_API __declspec(dllimport)
#endif

#include <iostream>
#include <string>
#include <vector>
#include <unordered_set>
#include <unordered_map>

#include <tiny_gltf.h>

#include "../../TEXTURE_KTX/TextureLoader.h"

class ENGINE_API GLTFMESHLoader
{
public:
	GLTFMESHLoader(const GLTFMESHLoader&) = delete;
	GLTFMESHLoader& operator=(const GLTFMESHLoader) = delete;

	static bool LoadGLTFModel(std::string filePath);
	static tinygltf::Model& GetGLTFModel(std::string fileName);
	static void ClearAllGLTFModels();

private:
	static std::unordered_map<std::string, tinygltf::Model> GLTFModels;
};
