#pragma once

#include <iostream>
#include <vector>
#include <set>
#include <unordered_set>
#include <unordered_map>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../../SHADERS/Shader.h"
#include "../../CAMERA/Camera.h"
#include "../../TEXTURE_KTX/TextureLoader.h"

constexpr int MAX_MESH_VERTEX_DATA = 1000;

struct DrawElementsIndirectCommand
{
    DrawElementsIndirectCommand() = default;

    DrawElementsIndirectCommand(GLuint vertex_count, GLuint instance_count
        , GLuint first_index, GLuint base_vertex, GLuint base_instance)
    {
        vertexCount = vertex_count;
        instanceCount = instance_count;
        firstIndex = first_index;
        baseVertex = base_vertex;
        baseInstance = base_instance;
    }

    GLuint vertexCount = 0;
    GLuint instanceCount = 0;
    GLuint firstIndex = 0;
    GLuint baseVertex = 0;
    GLuint baseInstance = 0;
};

struct MeshStructureForRendering
{
    std::string meshName;
    int meshSize;
    int meshInstances;
    int meshVertexBufferOffset;
    int meshIndexBufferOffset;
};

struct MeshOrientation
{
    glm::vec4 Position;
    glm::vec4 Rotation;
    glm::vec4 Scale;

    MeshOrientation(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale)
        :
        Position{ glm::vec4(position.x, position.y, position.z, 1.f) },
        Rotation{ glm::vec4(rotation.x, rotation.y, rotation.z, 1.f) },
        Scale{ glm::vec4(scale.x, scale.y, scale.z, 1.f) }
    {
    }
};

class BasicMeshRendererInstantiated
{
public:
    BasicMeshRendererInstantiated();
    ~BasicMeshRendererInstantiated();

    void AddMesh(std::string meshName, MeshOrientation meshOrientation);
    void CleanUp();
    void Render();

private:
    GLuint BasicMeshVAO, BasicMeshVBO, BasicMeshEBO;
    GLuint BasicMeshOrientationSSBO, IndirectCommandBuffer;

    void setupBuffers();

    bool IsBuffersUpdateRequired;
    int meshVerticesBufferOffset;
    int meshIndicesBufferOffset;

    std::vector<float> meshVertices;
    std::vector<unsigned int> meshIndices;
    std::unordered_map<std::string, MeshStructureForRendering> meshStructureForRendering;

    std::vector<DrawElementsIndirectCommand> indirectCommandBuffer;
    std::unordered_map<std::string, std::vector<MeshOrientation>> meshesOrientationPerMesh;
    std::vector<MeshOrientation> meshesOrientation;
};