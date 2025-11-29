#include "GLTFMESHLoader.h"
#include <iostream>

// Define the static member
std::unordered_map<std::string, tinygltf::Model> GLTFMESHLoader::GLTFModels;

bool GLTFMESHLoader::LoadGLTFModel(std::string filePath, bool IsAnimationPresent)
{
    tinygltf::TinyGLTF loader;
    tinygltf::Model model;
    std::string err, warn;

    bool ret = false;

    // Choose LoadASCIIFromFile or LoadBinaryFromFile depending on extension
    if (filePath.size() >= 5 && filePath.substr(filePath.size() - 5) == ".gltf")
    {
        ret = loader.LoadASCIIFromFile(&model, &err, &warn, filePath);
    }
    else if (filePath.size() >= 4 && filePath.substr(filePath.size() - 4) == ".glb")
    {
        ret = loader.LoadBinaryFromFile(&model, &err, &warn, filePath);
    }
    else
    {
        std::cerr << "Unsupported file format: " << filePath << "\n";
        return false;
    }

    if (!warn.empty())
        std::cout << "[GLTF Loader] WARN: " << warn << "\n";

    if (!err.empty())
        std::cerr << "[GLTF Loader] ERR: " << err << "\n";

    if (!ret)
    {
        std::cerr << "[GLTF Loader] Failed to load: " << filePath << "\n";
        return false;
    }

    // Extract just the file name (key for the map)
    std::string fileName = filePath.substr(filePath.find_last_of("/\\") + 1);

    std::string directory = filePath.substr(0, filePath.find_last_of("/\\") + 1);

    std::cout << "Number of Textures : " << model.textures.size() << std::endl;

    for (size_t i = 0; i < model.textures.size(); i++)
    {
        const tinygltf::Texture& tex = model.textures[i];
        const tinygltf::Image& image = model.images[tex.source];

        std::string baseColorTexturePath = directory 
            + (image.uri).substr(0, (image.uri).find_last_of('.')) + ".ktx2";

        TextureLoader::AddKTX2Texture(baseColorTexturePath
            , image.uri.substr(0, image.uri.find_last_of('.')), TextureType::NORMAL{});

        std::cout << baseColorTexturePath << std::endl;

        std::cout << image.uri << std::endl;


    }

    if (IsAnimationPresent)
    {
        if (!GLTFMESHSkeletalAnimationLoader::LoadSkeletalAnimationsFromGLTFModel(fileName, model))
        {
            std::cout << "[GLTFMESHLoader] Failed to load the Animations!." << std::endl;
        }
    }

    // Store in map
    GLTFModels[fileName] = std::move(model);

    std::cout << "[GLTF Loader] Loaded: " << fileName << "\n";
    return true;
}

tinygltf::Model& GLTFMESHLoader::GetGLTFModel(std::string fileName)
{
    auto it = GLTFModels.find(fileName);
    if (it == GLTFModels.end())
    {
        throw std::runtime_error("[GLTF Loader] Model not found: " + fileName);
    }
    return it->second;
}

void GLTFMESHLoader::ClearAllGLTFModels()
{
    std::cout << "[GLTF Loader] Clearing all loaded models ("
        << GLTFModels.size() << " models)...\n";
    GLTFModels.clear();
    std::cout << "[GLTF Loader] All models cleared.\n";
}
