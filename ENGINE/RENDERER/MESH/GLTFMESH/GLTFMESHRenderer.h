#pragma once

#ifdef ENGINE_EXPORTS
#define ENGINE_API __declspec(dllexport)
#else
#define ENGINE_API __declspec(dllimport)
#endif

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <tiny_gltf.h>
#include "GLTFMESHLoader.h"
#include "GLTFMESHSkeletalAnimationLoader.h"
#include "../../SHADERS/Shader.h"

#include "PROFILING/Profiler.h"

// Limits - tune to your needs
constexpr size_t MAX_TRIANGLES = 1000000; // reduce for safety if you want
constexpr size_t MAX_POS_VERTICES = MAX_TRIANGLES * 3;      // count of vertices (not floats)
constexpr size_t MAX_NORM_VERTICES = MAX_TRIANGLES * 3;
constexpr size_t MAX_TEXCOORD_VERTICES = MAX_TRIANGLES * 3;
constexpr size_t MAX_INDICES = MAX_TRIANGLES * 3;
constexpr size_t MAX_JOINTS_PER_VERTEX = MAX_TRIANGLES * 4;
constexpr size_t MAX_WEIGHTS_PER_VERTEX = MAX_TRIANGLES * 4;

struct ENGINE_API DrawElementsIndirectCommand
{
    DrawElementsIndirectCommand() = default;
    DrawElementsIndirectCommand(GLuint _count, GLuint _primCount, GLuint _firstIndex, GLuint _baseVertex, GLuint _baseInstance)
        : count(_count), primCount(_primCount), firstIndex(_firstIndex), baseVertex(_baseVertex), baseInstance(_baseInstance) {}

    GLuint count = 0;        // number of indices
    GLuint primCount = 0;    // instance count
    GLuint firstIndex = 0;   // offset into index buffer (in indices)
    GLuint baseVertex = 0;   // add to each index when reading vertex attributes
    GLuint baseInstance = 0; // first instance ID
};

struct GLTFModelOrientation
{
    // store as vec4 for easy SSBO upload
    glm::vec4 Position;
    glm::vec4 Rotation; // Euler degrees (temporarily). Ideally use quaternion / matrix.
    glm::vec3 Scale;
    int materialIndex;
    int animationIndex;
    glm::vec3 temp; // Temporary. To maintain the 16bit stride.

    GLTFModelOrientation() = default;

    GLTFModelOrientation(const glm::vec3& p, const glm::vec3& r, const glm::vec3& s, int materialIndex, int animationIndex)
        : 
        Position(p, 1.0f), 
        Rotation(r, 1.0f), 
        Scale(s), 
        materialIndex{materialIndex},
        animationIndex{animationIndex},
        temp{glm::vec3{1.f}}
    {
    }
};

struct GLTFMaterial
{
    int materialBindingIndex; // Stores the input of GL_TEXTURE_0
    int temp1;
    int temt2;
    int temp3;

    GLTFMaterial() = default;
    GLTFMaterial(const int materialBindingIndex)
        :
        materialBindingIndex{materialBindingIndex}
    {

    }
};

struct GLTFAnimations
{
    std::vector<glm::mat4> jointMatrices;

    GLTFAnimations()
        : jointMatrices(MAX_JOINTS, glm::mat4(1.0f))
    {
    }
};

struct MeshStructureForRendering
{
    std::string meshName;
    size_t meshIndexCnt = 0; // number of indices
    size_t meshInstances = 0;
    size_t meshPositionVertexOffset = 0; // baseVertex (in vertices)
    size_t meshIndexBufferOffset = 0;    // firstIndex (in indices)
};

class GLTFMESHRenderer
{
public:
    GLTFMESHRenderer();
    ~GLTFMESHRenderer();

    // Build and upload staging CPU-side data for given loaded glTF model
    // modelName should match the key used when calling GLTFMESHLoader::LoadGLTFModel()
    bool AddGLTFModelToRenderer(const std::string& modelName, const GLTFModelOrientation& orientation);

    // Render all added meshes (uploads changed buffers automatically)
    void GLTFMESHRender();

    // Clear CPU-side containers and GPU buffers (keeps VAO created, but empties internal state)
    void CleanUp();

    // Experimental
    void ExperimentalHelper();

private:
    // GPU objects
    GLuint meshVAO = 0;
    GLuint meshEBO = 0;
    GLuint meshPosVBO = 0;
    GLuint meshNormVBO = 0;
    GLuint meshTexVBO = 0;
    GLuint meshJointsVBO = 0;
    GLuint meshWeightsVBO = 0;

    GLuint OrientationSSBO = 0;
    GLuint MaterialSSBO = 0;
    GLuint IndirectCommandBuffer = 0;

    int GlobalMaterialTextureBindingIndex = 0;

    // CPU-side staging data
    std::vector<float> cpuPositions;   // contiguous floats (x,y,z) per vertex
    std::vector<float> cpuNormals;     // contiguous floats (x,y,z) per vertex
    std::vector<float> cpuTexcoords;   // contiguous floats (u,v) per vertex
    std::vector<int> cpuJoints;
    std::vector<float> cpuWeights;
    std::vector<uint32_t> cpuIndices;  // always convert indices to uint32_t

    // Bookkeeping offsets (in counts, not bytes)
    size_t positionsVertexCountOffset = 0; // how many vertices already present
    size_t indicesCountOffset = 0;

    // Structures describing each mesh/primitive
    std::unordered_map<std::string, MeshStructureForRendering> meshStructureForRendering;
    std::unordered_map<std::string, std::vector<GLTFModelOrientation>> primitivesOrientationPerMesh;
    std::unordered_map<std::string, GLTFMaterial> gltfMaterialMapping;

    // Indirect draw commands (built from meshStructureForRendering + orientations)
    std::vector<DrawElementsIndirectCommand> indirectCommands;

    // Flags
    bool IsBuffersUpdateRequired = false;

    // helpers
    void SetupGLTFMESHRenderer();
    void uploadBuffersIfRequired();
    static void copyAccessorToFloatVector(const tinygltf::Model& model, const tinygltf::Accessor& accessor, std::vector<float>& out, int expectedNumComponents);
    static void copyAccessorToIndexVector(const tinygltf::Model& model, const tinygltf::Accessor& accessor, std::vector<uint32_t>& out, uint32_t indexOffset);
    static void copyAccessorToIntVector(const tinygltf::Model& model, const tinygltf::Accessor& accessor, std::vector<int>& out);

    void ProcessNode(int nodeIdx, std::string modelName, tinygltf::Model& model, const GLTFModelOrientation& gltfModelOrientation);
};
