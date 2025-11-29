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

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>

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

    // Temporary
    // Helper: Finds the keyframe index and interpolation fraction for a specific timestamp
    int findKeyframe(const std::vector<float>& times, float animationTime, float& fraction) {
        if (times.empty()) return -1;

        // Handle out of bounds
        if (times.size() == 1 || animationTime <= times[0]) {
            fraction = 0.0f;
            return 0;
        }
        if (animationTime >= times.back()) {
            fraction = 0.0f;
            return (int)times.size() - 1;
        }

        // Linear search (sufficient for < 100 keyframes)
        for (size_t i = 0; i < times.size() - 1; ++i) {
            if (animationTime >= times[i] && animationTime < times[i + 1]) {
                float tStart = times[i];
                float tEnd = times[i + 1];
                fraction = (animationTime - tStart) / (tEnd - tStart);
                return (int)i;
            }
        }

        fraction = 0.0f;
        return 0;
    }

    std::vector<glm::mat4> getJointMatrices(float time) {
        std::vector<glm::mat4> jointMatrices(MAX_JOINTS, glm::mat4(1.0f));
        if (numJoints == 0) return jointMatrices;

        // Cycle the animation
        float animTime = 0.0f;
        if (animationsDurations > 0.0f) {
            animTime = fmod(time, animationsDurations);
        }

        std::vector<glm::mat4> globalTransforms(numJoints);

        for (int i = 0; i < numJoints; ++i) {
            // Start with Rest Pose
            glm::vec3 translation = restTranslations[i];
            glm::quat rotation = restRotations[i];
            glm::vec3 scale = restScales[i];

            float t = 0.0f;
            int k = 0;

            // 1. Sample Translation
            if (!translations[i].empty()) {
                k = findKeyframe(translationTimes[i], animTime, t);
                if (k < translations[i].size() - 1)
                    translation = glm::mix(translations[i][k], translations[i][k + 1], t);
                else
                    translation = translations[i][k];
            }

            // 2. Sample Rotation
            if (!rotations[i].empty()) {
                k = findKeyframe(rotationTimes[i], animTime, t);
                if (k < rotations[i].size() - 1)
                    rotation = glm::slerp(rotations[i][k], rotations[i][k + 1], t);
                else
                    rotation = rotations[i][k];
            }

            // 3. Sample Scale
            if (!scales[i].empty()) {
                k = findKeyframe(scaleTimes[i], animTime, t);
                if (k < scales[i].size() - 1)
                    scale = glm::mix(scales[i][k], scales[i][k + 1], t);
                else
                    scale = scales[i][k];
            }

            // Build Local Transform
            glm::mat4 localTransform = glm::translate(glm::mat4(1.0f), translation) * glm::toMat4(rotation) * glm::scale(glm::mat4(1.0f), scale);

            // Hierarchy transform (Parent * Child)
            if (jointNodeParents[i] == -1) {
                globalTransforms[i] = localTransform;
            }
            else {
                // Relies on parents having lower indices than children (standard in glTF skins)
                globalTransforms[i] = globalTransforms[jointNodeParents[i]] * localTransform;
            }

            // Final skinning matrix = Global * InverseBind
            jointMatrices[i] = globalTransforms[i] * inverseBindMatrices[i];
        }

        return jointMatrices;
    }

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
