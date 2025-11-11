#pragma once

#ifdef ENGINE_EXPORTS
#define ENGINE_API __declspec(dllexport)
#else
#define ENGINE_API __declspec(dllimport)
#endif

#include <memory>
#include <map>

#include "TextureKTX2.h"

class ENGINE_API TextureLoader
{
public:
	static std::shared_ptr<TextureKTX2> AddKTX2Texture(std::string path, std::string texture_name, struct TextureType::NORMAL);
	static std::shared_ptr<TextureKTX2> AddKTX2Texture(std::string path, std::string texture_name, struct TextureType::CUBEMAP);
	static std::shared_ptr<TextureKTX2> GetKTX2Texture(std::string texture_name);

	static void RemoveKTX2Texture(std::string texture_name);

	static void ClearAllKTX2Textures();
private:
	static std::map<std::string, std::pair<std::string, std::shared_ptr<TextureKTX2>>> textureKTX2Map; // <name, <path, shared_ptr>>
};