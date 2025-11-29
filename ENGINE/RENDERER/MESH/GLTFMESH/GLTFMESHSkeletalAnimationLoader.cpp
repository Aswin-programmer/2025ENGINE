#include "GLTFMESHSkeletalAnimationLoader.h"

#include <cstring> // for memcpy
#include <algorithm>

std::unordered_map<std::string
    , SkeletalAnimationData> GLTFMESHSkeletalAnimationLoader::SkeletalAnimations;

bool GLTFMESHSkeletalAnimationLoader::LoadSkeletalAnimationsFromGLTFModel(
    const std::string& gltfModelName
    , const tinygltf::Model& model
)
{
    // Already loaded?
    auto it = SkeletalAnimations.find(gltfModelName);
    if (it != SkeletalAnimations.end())
    {
        // Skeletal Animations have already been loaded.
        std::cout<< "[GLTFMESHAnimationLoader] " << gltfModelName << " is already loaded!." << std::endl;
        return false;
    }

    // Introducting some temp variables:
    // Skinning Data
    std::vector<glm::mat4> inverseBindMatrices;
    std::vector<int> jointParents;
    int numJoints = 0;

    // --- ANIMATION DATA (THE FIX) ---
    std::vector<std::vector<glm::vec3>> translations;
    std::vector<std::vector<float>> translationTimes; // Timestamps for translation

    std::vector<std::vector<glm::quat>> rotations;
    std::vector<std::vector<float>> rotationTimes;    // Timestamps for rotation

    std::vector<std::vector<glm::vec3>> scales;
    std::vector<std::vector<float>> scaleTimes;       // Timestamps for scale

    // Rest Pose Data
    std::vector<glm::vec3> restTranslations;
    std::vector<glm::quat> restRotations;
    std::vector<glm::vec3> restScales;

    // Max duration of the animation found in the file
    float animationDuration = 0.0f;

    // Read Skin, Hierarchy and the Rest Pose
    if (!model.skins.empty())
    {
        const tinygltf::Skin& skin = model.skins[0];
        numJoints = static_cast<int>(skin.joints.size());
        if (numJoints > MAX_JOINTS) numJoints = MAX_JOINTS;

        inverseBindMatrices.resize(numJoints, glm::mat4(1.0f));
        restTranslations.resize(numJoints, glm::vec3(0.0f));
        restRotations.resize(numJoints, glm::quat(1.0f, 0.0f, 0.0f, 0.0f));
        restScales.resize(numJoints, glm::vec3(1.0f));

        // Load IBM
        if (skin.inverseBindMatrices >= 0) {
            const tinygltf::Accessor& ibmAcc = model.accessors[skin.inverseBindMatrices];
            const unsigned char* ibmBase = GetBufferPointer(model, ibmAcc);
            if (ibmBase != nullptr && ibmAcc.count > 0) {
                const size_t expectedCount = std::min<size_t>(
                    ibmAcc.count, static_cast<size_t>(numJoints)
                );
                for (size_t i = 0; i < expectedCount; ++i) {
                    const float* m = reinterpret_cast<const float*>(
                        ibmBase + i * sizeof(float) * 16
                        );
                    inverseBindMatrices[i] = glm::make_mat4(m);
                }
            }
        }

        // Build Hierarchy & Extract Rest Pose
        jointParents.assign(numJoints, -1);
        for (int i = 0; i < numJoints; ++i) {
            int nodeIndex = skin.joints[i];
            if (nodeIndex < 0 || nodeIndex >= static_cast<int>(model.nodes.size())) continue;
            const tinygltf::Node& node = model.nodes[nodeIndex];

            if (node.translation.size() == 3)
                restTranslations[i] = glm::vec3(
                    (float)node.translation[0],
                    (float)node.translation[1],
                    (float)node.translation[2]
                );

            if (node.rotation.size() == 4)
                restRotations[i] = glm::quat(
                    (float)node.rotation[3], 
                    (float)node.rotation[0], 
                    (float)node.rotation[1], 
                    (float)node.rotation[2]
                );

            if (node.scale.size() == 3)
                restScales[i] = glm::vec3(
                    (float)node.scale[0], 
                    (float)node.scale[1], 
                    (float)node.scale[2]
                );

            for (int childNodeIdx : node.children) {
                for (int j = 0; j < numJoints; ++j) {
                    if (skin.joints[j] == childNodeIdx) {
                        jointParents[j] = i;
                        break;
                    }
                }
            }
        }
    }

    // ------------------------------------------------
    // PART 3: LOAD ANIMATION (CORRECTED)
    // ------------------------------------------------
    translations.resize(numJoints);
    translationTimes.resize(numJoints);
    rotations.resize(numJoints);
    rotationTimes.resize(numJoints);
    scales.resize(numJoints);
    scaleTimes.resize(numJoints);

    if (!model.animations.empty() && numJoints > 0) {
        const tinygltf::Animation& anim = model.animations[0];

        for (const auto& channel : anim.channels) {
            if (channel.sampler < 0 || channel.sampler >= static_cast<int>(anim.samplers.size())) continue;
            const tinygltf::AnimationSampler& sampler = anim.samplers[channel.sampler];

            // Find which joint this channel affects
            int jointIdx = -1;
            for (int j = 0; j < numJoints; ++j) {
                if (model.skins.size() > 0 && model.skins[0].joints[j] == channel.target_node) 
                { 
                    jointIdx = j; 
                    break; 
                }
            }
            if (jointIdx == -1) continue;

            // --- READ TIMINGS (PER CHANNEL) ---
            if (sampler.input < 0 || sampler.input >= static_cast<int>(model.accessors.size())) continue;
            const tinygltf::Accessor& timeAcc = model.accessors[sampler.input];
            const float* times = reinterpret_cast<const float*>(GetBufferPointer(model, timeAcc));
            if (!times) continue;

            std::vector<float> timeData;
            timeData.resize(timeAcc.count);
            for (size_t k = 0; k < timeAcc.count; ++k) {
                timeData[k] = times[k];
                // Update global animation duration
                if (times[k] > animationDuration) animationDuration = times[k];
            }

            // --- READ VALUES ---
            if (sampler.output < 0 || sampler.output >= static_cast<int>(model.accessors.size())) continue;
            const tinygltf::Accessor& outAcc = model.accessors[sampler.output];
            const unsigned char* outData = GetBufferPointer(model, outAcc);
            if (!outData) continue;
            const float* vecData = reinterpret_cast<const float*>(outData);

            // Ensure outAcc.count matches timeAcc.count in well-formed files (guard)
            const size_t keyframeCount = std::min<size_t>(outAcc.count, timeAcc.count);

            if (channel.target_path == "translation") {
                translationTimes[jointIdx] = std::move(timeData); // Store specific times
                translations[jointIdx].resize(keyframeCount);
                for (size_t k = 0; k < keyframeCount; ++k) {
                    // out accessor for translations are vec3
                    const size_t base = k * 3;
                    translations[jointIdx][k] =
                        glm::vec3(vecData[base], vecData[base + 1], vecData[base + 2]);
                }
            }
            else if (channel.target_path == "rotation") {
                rotationTimes[jointIdx] = std::move(timeData); // Store specific times
                rotations[jointIdx].resize(keyframeCount);
                for (size_t k = 0; k < keyframeCount; ++k) {
                    // out accessor for rotations are vec4 (x, y, z, w)
                    const size_t base = k * 4;
                    // tinygltf stores quaternion as x,y,z,w -> glm::quat(w, x, y, z)
                    rotations[jointIdx][k] =
                        glm::quat(vecData[base + 3], vecData[base + 0], vecData[base + 1], vecData[base + 2]);
                }
            }
            else if (channel.target_path == "scale") {
                scaleTimes[jointIdx] = std::move(timeData); // Store specific times
                scales[jointIdx].resize(keyframeCount);
                for (size_t k = 0; k < keyframeCount; ++k) {
                    const size_t base = k * 3;
                    scales[jointIdx][k] =
                        glm::vec3(vecData[base], vecData[base + 1], vecData[base + 2]);
                }
            }
        }
    }

    // Construct SkeletalAnimationData and store in static map
    SkeletalAnimationData data(
        gltfModelName,
        std::move(inverseBindMatrices),
        std::move(jointParents),
        numJoints,
        std::move(translations),
        std::move(translationTimes),
        std::move(rotations),
        std::move(rotationTimes),
        std::move(scales),
        std::move(scaleTimes),
        std::move(restTranslations),
        std::move(restRotations),
        std::move(restScales),
        animationDuration
    );

    // Insert into the global map
    SkeletalAnimations.emplace(gltfModelName, std::move(data));

    std::cout << "[GLTFMESHAnimationLoader] " << gltfModelName << " Loaded successfully" << std::endl;

    return true;
}

void GLTFMESHSkeletalAnimationLoader::DeleteAllGLTFMESHSkeletalAnimations()
{
    // Clear the map and free memory
    SkeletalAnimations.clear();
    SkeletalAnimations.rehash(0); // optional: release buckets
}

SkeletalAnimationData& GLTFMESHSkeletalAnimationLoader::GetGLTFMESHSkeletalAnimations(
    const std::string& gltfModelName
)
{
    auto it = SkeletalAnimations.find(gltfModelName);
    if (it == SkeletalAnimations.end()) {
        throw std::out_of_range("GLTF mesh skeletal animation not found: " + gltfModelName);
    }
    return it->second;
}

const unsigned char* GLTFMESHSkeletalAnimationLoader::GetBufferPointer(const tinygltf::Model& model, const tinygltf::Accessor& acc)
{
    if (acc.bufferView < 0) return nullptr;
    if (acc.bufferView >= static_cast<int>(model.bufferViews.size())) return nullptr;
    const tinygltf::BufferView& view = model.bufferViews[acc.bufferView];
    if (view.buffer < 0 || view.buffer >= static_cast<int>(model.buffers.size())) return nullptr;
    const tinygltf::Buffer& buffer = model.buffers[view.buffer];

    const size_t viewOffset = static_cast<size_t>(view.byteOffset);
    const size_t accOffset = static_cast<size_t>(acc.byteOffset);
    if (viewOffset + accOffset >= buffer.data.size()) return nullptr;

    return buffer.data.data() + (viewOffset + accOffset);
}

size_t GLTFMESHSkeletalAnimationLoader::GetByteStride(const tinygltf::Model& model, const tinygltf::Accessor& acc)
{
    if (acc.bufferView < 0) return 0;
    if (acc.bufferView >= static_cast<int>(model.bufferViews.size())) return 0;
    const tinygltf::BufferView& view = model.bufferViews[acc.bufferView];
    return static_cast<size_t>(view.byteStride);
}
