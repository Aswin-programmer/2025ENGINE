#include "BasicMeshRendererInstantiated.h"

std::vector<float> CubeVertices = {
    // Positions          / Texture Coords
    // Front face
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    // Back face
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    // Left face
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    // Right face
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

     // Bottom face
     -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
      0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
      0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
      0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

     // Top face
     -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
      0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
      0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
      0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
     -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};

std::vector<unsigned int> CubeIndices = {
    // Front face
    0, 1, 2,
    3, 4, 5,

    // Back face
    6, 7, 8,
    9, 10, 11,

    // Left face
    12, 13, 14,
    15, 16, 17,

    // Right face
    18, 19, 20,
    21, 22, 23,

    // Bottom face
    24, 25, 26,
    27, 28, 29,

    // Top face
    30, 31, 32,
    33, 34, 35
};

std::vector<float> PyramidVertices = {
    // Base face (two triangles)
    -0.5f, 0.0f, -0.5f,  0.0f, 0.0f,
     0.5f, 0.0f, -0.5f,  1.0f, 0.0f,
     0.5f, 0.0f,  0.5f,  1.0f, 1.0f,
     0.5f, 0.0f,  0.5f,  1.0f, 1.0f,
    -0.5f, 0.0f,  0.5f,  0.0f, 1.0f,
    -0.5f, 0.0f, -0.5f,  0.0f, 0.0f,

    // Side face 1 (Front)
    -0.5f, 0.0f, -0.5f,  0.0f, 0.0f,
     0.5f, 0.0f, -0.5f,  1.0f, 0.0f,
     0.0f, 1.0f,  0.0f,  0.5f, 1.0f,

     // Side face 2 (Right)
      0.5f, 0.0f, -0.5f,  0.0f, 0.0f,
      0.5f, 0.0f,  0.5f,  1.0f, 0.0f,
      0.0f, 1.0f,  0.0f,  0.5f, 1.0f,

      // Side face 3 (Back)
       0.5f, 0.0f,  0.5f,  1.0f, 0.0f,
      -0.5f, 0.0f,  0.5f,  0.0f, 0.0f,
       0.0f, 1.0f,  0.0f,  0.5f, 1.0f,

       // Side face 4 (Left)
       -0.5f, 0.0f,  0.5f,  1.0f, 0.0f,
       -0.5f, 0.0f, -0.5f,  0.0f, 0.0f,
        0.0f, 1.0f,  0.0f,  0.5f, 1.0f
};

std::vector<unsigned int> PyramidIndices = {
    // Base
    0, 1, 2,
    3, 4, 5,

    // Sides
    6, 7, 8,     // Front
    9, 10,11,    // Right
    12,13,14,    // Back
    15,16,17     // Left
};

BasicMeshRendererInstantiated::BasicMeshRendererInstantiated()
    :
    BasicMeshVAO{ 0 },
    BasicMeshVBO{ 0 },
    BasicMeshEBO{ 0 },
    BasicMeshOrientationSSBO{ 0 },
    IndirectCommandBuffer{ 0 },
    meshVerticesBufferOffset{ 0 },
    meshIndicesBufferOffset{ 0 },
    IsBuffersUpdateRequired{ false }
{
    setupBuffers();
}

BasicMeshRendererInstantiated::~BasicMeshRendererInstantiated()
{
    glDeleteBuffers(1, &BasicMeshVBO);
    glDeleteBuffers(1, &BasicMeshEBO);
    glDeleteBuffers(1, &BasicMeshOrientationSSBO);
    glDeleteBuffers(1, &IndirectCommandBuffer);
    glDeleteVertexArrays(1, &BasicMeshVAO);
}

void BasicMeshRendererInstantiated::setupBuffers()
{
    glCreateVertexArrays(1, &BasicMeshVAO);

    // Vertex Buffer Configuration
    glCreateBuffers(1, &BasicMeshVBO);
    glNamedBufferStorage(BasicMeshVBO, sizeof(float) * 4000000, nullptr, GL_DYNAMIC_STORAGE_BIT);

    glVertexArrayVertexBuffer(BasicMeshVAO, 0, BasicMeshVBO, 0, sizeof(float) * 5);

    glVertexArrayAttribFormat(BasicMeshVAO, 0, 3, GL_FLOAT, GL_FALSE, 0);
    glVertexArrayAttribBinding(BasicMeshVAO, 0, 0);
    glEnableVertexArrayAttrib(BasicMeshVAO, 0);

    glVertexArrayAttribFormat(BasicMeshVAO, 1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 3);
    glVertexArrayAttribBinding(BasicMeshVAO, 1, 0);
    glEnableVertexArrayAttrib(BasicMeshVAO, 1);

    // Index buffers configuration
    glCreateBuffers(1, &BasicMeshEBO);
    glNamedBufferStorage(BasicMeshEBO, sizeof(unsigned int) * 800000, nullptr, GL_DYNAMIC_STORAGE_BIT);
    glVertexArrayElementBuffer(BasicMeshVAO, BasicMeshEBO);

    // Mesh Orientation Configuration Using Shader Storage Buffer
    glCreateBuffers(1, &BasicMeshOrientationSSBO);
    glNamedBufferStorage(BasicMeshOrientationSSBO, sizeof(MeshOrientation) * 20000, nullptr, GL_DYNAMIC_STORAGE_BIT);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, BasicMeshOrientationSSBO);

    // Multi Draw Command configuration
    glCreateBuffers(1, &IndirectCommandBuffer);
    glNamedBufferStorage(IndirectCommandBuffer, sizeof(DrawElementsIndirectCommand) * 100, nullptr, GL_DYNAMIC_STORAGE_BIT);
}

void BasicMeshRendererInstantiated::CleanUp()
{
    meshStructureForRendering.clear();
    meshesOrientationPerMesh.clear();
    meshesOrientation.clear();
    indirectCommandBuffer.clear();
    meshVertices.clear();
    meshIndices.clear();
    meshVerticesBufferOffset = 0;
    meshIndicesBufferOffset = 0;
    IsBuffersUpdateRequired = false;
}

void BasicMeshRendererInstantiated::AddMesh(std::string meshName, MeshOrientation meshOrientation)
{
    auto itr = meshStructureForRendering.find(meshName);
    if (itr == meshStructureForRendering.end())
    {
        meshStructureForRendering[meshName] = MeshStructureForRendering{
            .meshName = meshName,
            .meshInstances = 1,
            .meshVertexBufferOffset = meshVerticesBufferOffset,
            .meshIndexBufferOffset = meshIndicesBufferOffset
        };

        if (meshName == "CUBE")
        {
            meshVertices.insert(meshVertices.end(), CubeVertices.begin(), CubeVertices.end());
            meshIndices.insert(meshIndices.end(), CubeIndices.begin(), CubeIndices.end());
            meshStructureForRendering[meshName].meshSize = CubeIndices.size();
        }
        else if (meshName == "PYRAMID")
        {
            meshVertices.insert(meshVertices.end(), PyramidVertices.begin(), PyramidVertices.end());
            meshIndices.insert(meshIndices.end(), PyramidIndices.begin(), PyramidIndices.end());
            meshStructureForRendering[meshName].meshSize = PyramidIndices.size();
        }

        meshVerticesBufferOffset = meshVertices.size() / 5;
        meshIndicesBufferOffset = meshIndices.size();
        IsBuffersUpdateRequired = true;
    }
    else
    {
        meshStructureForRendering[meshName].meshInstances += 1;
    }

    meshesOrientationPerMesh[meshName].push_back(meshOrientation);
}

void BasicMeshRendererInstantiated::Render()
{
    if (meshStructureForRendering.empty()) return;

    glBindVertexArray(BasicMeshVAO);

    if (IsBuffersUpdateRequired)
    {
        // Upload the vertex buffer
        glNamedBufferSubData(BasicMeshVBO, 0,
            meshVertices.size() * sizeof(float), meshVertices.data());
        // Upload the index buffer
        glNamedBufferSubData(BasicMeshEBO, 0,
            meshIndices.size() * sizeof(unsigned int), meshIndices.data());

        IsBuffersUpdateRequired = false;
    }

    int mesh_instances = 0;
    for (const auto& mesh : meshStructureForRendering)
    {
        const std::vector<MeshOrientation>& current_mesh = meshesOrientationPerMesh[mesh.second.meshName];

        meshesOrientation.insert(meshesOrientation.end(),
            current_mesh.begin(), current_mesh.end());

        indirectCommandBuffer.push_back(
            DrawElementsIndirectCommand(
                mesh.second.meshSize,
                mesh.second.meshInstances,
                mesh.second.meshIndexBufferOffset,
                mesh.second.meshVertexBufferOffset,
                mesh_instances
            )
        );
        mesh_instances += mesh.second.meshInstances;
    }

    // Upload orientation data
    if (!meshesOrientation.empty())
    {
        glNamedBufferSubData(BasicMeshOrientationSSBO, 0,
            meshesOrientation.size() * sizeof(MeshOrientation),
            meshesOrientation.data());
    }

    // Upload indirect commands
    if (!indirectCommandBuffer.empty())
    {
        glNamedBufferSubData(IndirectCommandBuffer, 0,
            indirectCommandBuffer.size() * sizeof(DrawElementsIndirectCommand),
            indirectCommandBuffer.data()); // ** &indirectCommandBuffer [THIS IS A VERY VERY BIG BUG] **

        glBindBuffer(GL_DRAW_INDIRECT_BUFFER, IndirectCommandBuffer);

        // Use multi-draw indirect for multiple draw commands
        if (indirectCommandBuffer.size() > 1)
        {
            glMultiDrawElementsIndirect(GL_TRIANGLES, GL_UNSIGNED_INT, 0,
                static_cast<GLsizei>(indirectCommandBuffer.size()), 0);
        }
        else
        {
            glDrawElementsIndirect(GL_TRIANGLES, GL_UNSIGNED_INT, 0); // This Alone Is Enough!.
        }
    }
}