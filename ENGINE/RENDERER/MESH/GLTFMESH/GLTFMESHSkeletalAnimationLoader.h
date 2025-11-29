#pragma once

#ifdef ENGINE_EXPORTS
#define ENGINE_API __declspec(dllexport)
#else
#define ENGINE_API __declspec(dllimport)
#endif

constexpr int MAX_JOINTS = 20;

#include <string>
#include <vector>
#include <unordered_map>
#include <iostream>

#include <tiny_gltf.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>

struct SkeletalAnimationData
{
    std::string name;

    std::vector<glm::mat4> inverseBindMatrices;
    std::vector<int> jointNodeParents;
    int numJoints = 0;

    std::vector<std::vector<glm::vec3>> translations;
    std::vector<std::vector<float>> translationTimes;

    std::vector<std::vector<glm::quat>> rotations;
    std::vector<std::vector<float>> rotationTimes;

    std::vector<std::vector<glm::vec3>> scales;
    std::vector<std::vector<float>> scaleTimes;

    std::vector<glm::vec3> restTranslations;
    std::vector<glm::quat> restRotations;
    std::vector<glm::vec3> restScales;

    float animationsDurations = 0.0f;

    SkeletalAnimationData(
        const std::string& name,
        std::vector<glm::mat4> inverseBindMatrices,
        std::vector<int> jointNodeParents,
        int numJoints,
        std::vector<std::vector<glm::vec3>> translations,
        std::vector<std::vector<float>> translationTimes,
        std::vector<std::vector<glm::quat>> rotations,
        std::vector<std::vector<float>> rotationTimes,
        std::vector<std::vector<glm::vec3>> scales,
        std::vector<std::vector<float>> scaleTimes,
        std::vector<glm::vec3> restTranslations,
        std::vector<glm::quat> restRotations,
        std::vector<glm::vec3> restScales,
        float animationsDurations
    )
        : name(name)
        , inverseBindMatrices(std::move(inverseBindMatrices))
        , jointNodeParents(std::move(jointNodeParents))
        , numJoints(numJoints)
        , translations(std::move(translations))
        , translationTimes(std::move(translationTimes))
        , rotations(std::move(rotations))
        , rotationTimes(std::move(rotationTimes))
        , scales(std::move(scales))
        , scaleTimes(std::move(scaleTimes))
        , restTranslations(std::move(restTranslations))
        , restRotations(std::move(restRotations))
        , restScales(std::move(restScales))
        , animationsDurations(animationsDurations)
    {
    }
};

class ENGINE_API GLTFMESHSkeletalAnimationLoader
{
public:
    // static-only utility class: delete ctor + copy/assign
    GLTFMESHSkeletalAnimationLoader() = delete;
    GLTFMESHSkeletalAnimationLoader(const GLTFMESHSkeletalAnimationLoader&) = delete;
    GLTFMESHSkeletalAnimationLoader& operator=(const GLTFMESHSkeletalAnimationLoader&) = delete;

    // Loads skeletal animations from a glTF model. Returns true on success (and stores into static map).
    // If animations for this model already exist, returns false.
    static bool LoadSkeletalAnimationsFromGLTFModel(const std::string& gltfModelName
        , const tinygltf::Model& model);

    // Access to loaded animations (read-only)
    static const std::unordered_map<std::string, SkeletalAnimationData>& GetSkeletalAnimations() { return SkeletalAnimations; }

    // Delete all loaded skeletal animations (clears the internal map)
    static void DeleteAllGLTFMESHSkeletalAnimations();

    // Get a reference to a loaded SkeletalAnimationData by model name.
    // Throws std::out_of_range if not found.
    static SkeletalAnimationData& GetGLTFMESHSkeletalAnimations(const std::string& gltfModelName);

private:
    static std::unordered_map<std::string, SkeletalAnimationData> SkeletalAnimations;

    // Helper Functions for Loading:
    static const unsigned char* GetBufferPointer(const tinygltf::Model& model
        , const tinygltf::Accessor& acc);

    static size_t GetByteStride(const tinygltf::Model& model
        , const tinygltf::Accessor& acc);
};
