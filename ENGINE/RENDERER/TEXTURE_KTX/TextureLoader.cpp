#include "TextureLoader.h"

std::map<std::string, std::pair<std::string, std::shared_ptr<TextureKTX2>>> TextureLoader::textureKTX2Map;

std::shared_ptr<TextureKTX2> TextureLoader::AddKTX2Texture(std::string path, std::string texture_name, struct TextureType::NORMAL)
{
	auto it = textureKTX2Map.find(texture_name);
	if (it == textureKTX2Map.end())
	{
		textureKTX2Map[texture_name] = std::make_pair(path, std::make_shared<TextureKTX2>(path, TextureType::NORMAL{}));
	}

	std::cout << "[Texture Loader] Loaded: " << texture_name << "\n";

	return textureKTX2Map[texture_name].second;
}

std::shared_ptr<TextureKTX2> TextureLoader::AddKTX2Texture(std::string path, std::string texture_name, struct TextureType::CUBEMAP)
{
	auto it = textureKTX2Map.find(texture_name);
	if (it == textureKTX2Map.end())
	{
		textureKTX2Map[texture_name] = std::make_pair(path, std::make_shared<TextureKTX2>(path, TextureType::CUBEMAP{}));
	}
	return textureKTX2Map[texture_name].second;
}

std::shared_ptr<TextureKTX2> TextureLoader::GetKTX2Texture(std::string texture_name)
{
	auto it = textureKTX2Map.find(texture_name);
	if (it == textureKTX2Map.end())
	{
		std::cout << "The texture [Name: " << texture_name << "] required is not added!" << std::endl;
	}
	return textureKTX2Map[texture_name].second;
}

void TextureLoader::RemoveKTX2Texture(std::string texture_name)
{
	auto it = textureKTX2Map.find(texture_name);
	if (it != textureKTX2Map.end())
	{
		// Erase the entry from the map
		textureKTX2Map.erase(it);
		std::cout << "Texture [" << texture_name << "] destroyed successfully!" << std::endl;
	}
	else
	{
		std::cout << "Texture [" << texture_name << "] not found, cannot destroy!" << std::endl;
	}
}

void TextureLoader::ClearAllKTX2Textures()
{
	if (!textureKTX2Map.empty())
	{
		textureKTX2Map.clear();  // All shared_ptrs are released
		std::cout << "All KTX2 textures destroyed!" << std::endl;
	}
	else
	{
		std::cout << "No textures to destroy." << std::endl;
	}
}

