#include "GLTFMESHRenderer.h"

#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define TINYGLTF_NOEXCEPTION
#define JSON_NOEXCEPTION
#define GLM_ENABLE_EXPERIMENTAL

#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define BUFFER_OFFSET(i) ((void*)(size_t)(i))

GLTFMESHRenderer::GLTFMESHRenderer()
{
    SetupGLTFMESHRenderer();
}

GLTFMESHRenderer::~GLTFMESHRenderer()
{
    // delete GPU objects
    if (meshPosVBO) glDeleteBuffers(1, &meshPosVBO);
    if (meshNormVBO) glDeleteBuffers(1, &meshNormVBO);
    if (meshTexVBO) glDeleteBuffers(1, &meshTexVBO);
    if (meshEBO) glDeleteBuffers(1, &meshEBO);
    if (OrientationSSBO) glDeleteBuffers(1, &OrientationSSBO);
    if (MaterialSSBO) glDeleteBuffers(1, &MaterialSSBO);
    if (IndirectCommandBuffer) glDeleteBuffers(1, &IndirectCommandBuffer);
    if (meshVAO) glDeleteVertexArrays(1, &meshVAO);
}

void GLTFMESHRenderer::SetupGLTFMESHRenderer()
{
    // Create VAO
    glCreateVertexArrays(1, &meshVAO);

    // POSITION VBO
    glCreateBuffers(1, &meshPosVBO);
    // reserve floats (x,y,z) * vertexCount
    glNamedBufferStorage(meshPosVBO, sizeof(float) * 3 * MAX_POS_VERTICES, nullptr, GL_DYNAMIC_STORAGE_BIT);
    // bind as binding index 0
    glVertexArrayVertexBuffer(meshVAO, 0, meshPosVBO, 0, sizeof(float) * 3);
    // attribute 0 -> position (vec3)
    glVertexArrayAttribFormat(meshVAO, 0, 3, GL_FLOAT, GL_FALSE, 0);
    glVertexArrayAttribBinding(meshVAO, 0, 0);
    glEnableVertexArrayAttrib(meshVAO, 0);

    // NORMAL VBO
    glCreateBuffers(1, &meshNormVBO);
    glNamedBufferStorage(meshNormVBO, sizeof(float) * 3 * MAX_NORM_VERTICES, nullptr, GL_DYNAMIC_STORAGE_BIT);
    glVertexArrayVertexBuffer(meshVAO, 1, meshNormVBO, 0, sizeof(float) * 3);
    // attribute 1 -> normal (vec3)
    glVertexArrayAttribFormat(meshVAO, 1, 3, GL_FLOAT, GL_FALSE, 0);
    glVertexArrayAttribBinding(meshVAO, 1, 1);
    glEnableVertexArrayAttrib(meshVAO, 1);

    // TEXCOORD VBO
    glCreateBuffers(1, &meshTexVBO);
    glNamedBufferStorage(meshTexVBO, sizeof(float) * 2 * MAX_TEXCOORD_VERTICES, nullptr, GL_DYNAMIC_STORAGE_BIT);
    glVertexArrayVertexBuffer(meshVAO, 2, meshTexVBO, 0, sizeof(float) * 2);
    // attribute 2 -> texcoord (vec2)
    glVertexArrayAttribFormat(meshVAO, 2, 2, GL_FLOAT, GL_FALSE, 0);
    glVertexArrayAttribBinding(meshVAO, 2, 2);
    glEnableVertexArrayAttrib(meshVAO, 2);

    // ELEMENT / INDEX buffer (we store 32-bit indices on GPU)
    glCreateBuffers(1, &meshEBO);
    glNamedBufferStorage(meshEBO, sizeof(uint32_t) * MAX_INDICES, nullptr, GL_DYNAMIC_STORAGE_BIT);
    glVertexArrayElementBuffer(meshVAO, meshEBO);

    // Orientation SSBO (binding 0)
    glCreateBuffers(1, &OrientationSSBO);
    // size for instances (parameterize if needed)
    glNamedBufferStorage(OrientationSSBO, sizeof(GLTFPrimitivesOrientation) * 20000, nullptr, GL_DYNAMIC_STORAGE_BIT);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, OrientationSSBO);

    // Material SSBO (binding 1)
    glCreateBuffers(1, &MaterialSSBO);
    // size for instances (parameterize if needed)
    glNamedBufferStorage(MaterialSSBO, sizeof(GLTFMaterial) * 20000, nullptr, GL_DYNAMIC_STORAGE_BIT);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, MaterialSSBO);

    // Indirect command buffer
    glCreateBuffers(1, &IndirectCommandBuffer);
    glNamedBufferStorage(IndirectCommandBuffer, sizeof(DrawElementsIndirectCommand) * 4096, nullptr, GL_DYNAMIC_STORAGE_BIT);
}

void GLTFMESHRenderer::CleanUp()
{
    meshStructureForRendering.clear();
    primitivesOrientationPerMesh.clear();
    GlobalMaterialTextureBindingIndex = 0;
    gltfMaterialMapping.clear();
    indirectCommands.clear();

    cpuPositions.clear();
    cpuNormals.clear();
    cpuTexcoords.clear();
    cpuIndices.clear();

    positionsVertexCountOffset = 0;
    indicesCountOffset = 0;
    IsBuffersUpdateRequired = false;
}

bool GLTFMESHRenderer::AddGLTFModelToRenderer(const std::string& modelName, const GLTFModelOrientation& gltfModelOrientation)
{
    // Get model by reference (no copy)
    tinygltf::Model& model = GLTFMESHLoader::GetGLTFModel(modelName);

    // Validate scene
    if (model.scenes.empty() || model.defaultScene < 0 || model.defaultScene >= static_cast<int>(model.scenes.size()))
    {
        std::cerr << "[GLTFMESHRenderer] Model has no valid default scene: " << modelName << "\n";
        return false;
    }

    const tinygltf::Scene& scene = model.scenes[model.defaultScene];

    for (int nodeIdx : scene.nodes)
    {
        const tinygltf::Node& node = model.nodes[nodeIdx];

        // build a key per node (could be per primitive if you prefer)
        std::string key = modelName + "_" + std::to_string(nodeIdx);

        // If this node already added, increment instances
        auto it = meshStructureForRendering.find(key);
        if (it != meshStructureForRendering.end())
        {
            meshStructureForRendering[key].meshInstances += 1;
        }
        else
        {
            // create new structure, fill offsets
            MeshStructureForRendering msr;
            msr.meshName = key;
            msr.meshInstances = 1;
            msr.meshPositionVertexOffset = positionsVertexCountOffset; // baseVertex for future draws
            msr.meshIndexBufferOffset = indicesCountOffset; // firstIndex for future draws

            // If node has a mesh, process its primitives
            if (node.mesh >= 0)
            {
                const tinygltf::Mesh& mesh = model.meshes[node.mesh];

                // We'll accumulate all primitives of this mesh as a single "mesh" for simplicity.
                size_t totalIndicesForThisMesh = 0;
                size_t totalVerticesForThisMesh = 0;

                for (const auto& primitive : mesh.primitives)
                {
                    // POSITION (required)
                    if (primitive.attributes.find("POSITION") == primitive.attributes.end())
                    {
                        std::cerr << "[GLTFMESHRenderer] Primitive missing POSITION attribute\n";
                        return false;
                    }

                    const tinygltf::Accessor& posAccessor = model.accessors[primitive.attributes.at("POSITION")];

                    // primitive base (vertex index offset) is the current global vertex count
                    uint32_t primitiveVertexBase = static_cast<uint32_t>(positionsVertexCountOffset);

                    // Copy positions first
                    copyAccessorToFloatVector(model, posAccessor, cpuPositions, 3);

                    // Advance global vertex count
                    positionsVertexCountOffset += posAccessor.count;
                    totalVerticesForThisMesh += posAccessor.count;

                    // Indices
                    if (primitive.indices >= 0)
                    {
                        const tinygltf::Accessor& indexAccessor = model.accessors[primitive.indices];
                        // copy indices but offset them by primitiveVertexBase
                        copyAccessorToIndexVector(model, indexAccessor, cpuIndices, primitiveVertexBase);

                        totalIndicesForThisMesh += indexAccessor.count;
                        indicesCountOffset += indexAccessor.count;
                    }
                    else
                    {
                        // If no indices (draw arrays) generate sequential indices with vertex base
                        for (uint32_t i = 0; i < static_cast<uint32_t>(posAccessor.count); ++i)
                            cpuIndices.push_back(primitiveVertexBase + i);

                        totalIndicesForThisMesh += posAccessor.count;
                        indicesCountOffset += posAccessor.count;
                    }

                    // NORMAL (optional) - append per-primitive zeros if missing
                    if (primitive.attributes.find("NORMAL") != primitive.attributes.end())
                    {
                        const tinygltf::Accessor& normAccessor = model.accessors[primitive.attributes.at("NORMAL")];
                        copyAccessorToFloatVector(model, normAccessor, cpuNormals, 3);
                    }
                    else
                    {
                        // push zeros for each vertex of this primitive
                        cpuNormals.insert(cpuNormals.end(), static_cast<size_t>(posAccessor.count) * 3, 0.0f);
                    }

                    // TEXCOORD_0 (optional)
                    if (primitive.attributes.find("TEXCOORD_0") != primitive.attributes.end())
                    {
                        const tinygltf::Accessor& texAccessor = model.accessors[primitive.attributes.at("TEXCOORD_0")];
                        copyAccessorToFloatVector(model, texAccessor, cpuTexcoords, 2);
                    }
                    else
                    {
                        // push zeros for each vertex of this primitive
                        cpuTexcoords.insert(cpuTexcoords.end(), static_cast<size_t>(posAccessor.count) * 2, 0.0f);
                    }

                } // primitive loop

                msr.meshIndexCnt = totalIndicesForThisMesh;
            }
            else
            {
                // Node without mesh -> skip adding structure
                continue;
            }

            meshStructureForRendering.emplace(key, msr);

            if (model.textures.size() > 0)
            {
                for (size_t i = 0; i < model.textures.size(); i++)
                {
                    const tinygltf::Texture& tex = model.textures[i];
                    const tinygltf::Image& image = model.images[tex.source];

                    std::string filename = image.uri;

                    // Find last underscore and last dot
                    size_t underscorePos = filename.find_last_of('_');
                    size_t dotPos = filename.find_last_of('.');

                    std::string result;
                    if (underscorePos != std::string::npos && dotPos != std::string::npos && underscorePos < dotPos) {
                        result = filename.substr(underscorePos + 1, dotPos - underscorePos - 1);
                    }

                    // Temporary:
                    if (result == "baseColor")
                    {
                        std::shared_ptr<TextureKTX2> texture = TextureLoader::GetKTX2Texture(image.uri.substr(0, image.uri.find_last_of('.')));
                        texture->Bind(GlobalMaterialTextureBindingIndex);
                        gltfMaterialMapping[key] = GLTFMaterial(GlobalMaterialTextureBindingIndex);
                        GlobalMaterialTextureBindingIndex++;
                    }
                }
            }
        }

        // Add material index to the model orientation
        int currentMaterialIndex = -1;
        if (gltfMaterialMapping.find(key) != gltfMaterialMapping.end())
        {
            currentMaterialIndex = gltfMaterialMapping[key].materialBindingIndex;
        }

        // compute local transform (position/rotation/scale)
        glm::vec3 localPos(0.0f);
        glm::vec3 localRotEuler(0.0f); // degrees
        glm::vec3 localScale(1.0f);

        if (node.translation.size() == 3)
        {
            localPos = glm::vec3(
                static_cast<float>(node.translation[0]),
                static_cast<float>(node.translation[1]),
                static_cast<float>(node.translation[2])
            );
        }

        if (node.rotation.size() == 4)
        {
            // tinygltf rotation is [x, y, z, w]
            // glm::quat constructor takes (w, x, y, z) when using individual floats.
            glm::quat q(
                static_cast<float>(node.rotation[3]), // w
                static_cast<float>(node.rotation[0]), // x
                static_cast<float>(node.rotation[1]), // y
                static_cast<float>(node.rotation[2])  // z
            );
            glm::vec3 euler = glm::degrees(glm::eulerAngles(q));
            localRotEuler = euler;
        }

        if (node.scale.size() == 3)
        {
            localScale = glm::vec3(
                static_cast<float>(node.scale[0]),
                static_cast<float>(node.scale[1]),
                static_cast<float>(node.scale[2])
            );
        }

        // Compose final orientation by simple addition for position/rotation and multiplication for scale.
        // NOTE: This is a simplification. Proper transform composition should be matrix/quaternion multiply.
        GLTFPrimitivesOrientation primOrient(
            glm::vec3(gltfModelOrientation.Position) + localPos,
            glm::vec3(gltfModelOrientation.Rotation) + localRotEuler,
            glm::vec3(gltfModelOrientation.Scale) * localScale,
            currentMaterialIndex
        );

        // Always push orientation for this instance
        primitivesOrientationPerMesh[key].push_back(primOrient);

    } // node loop

    // after adding model, signal buffers need upload
    IsBuffersUpdateRequired = true;
    return true;
}

void GLTFMESHRenderer::GLTFMESHRender()
{
    PROFILE_SCOPE_N("Renderer::render");

    if (indirectCommands.empty()) return;

    

#ifdef ENABLE_RENDERER_INFO
    GLuint queryID;
    glGenQueries(1, &queryID);

    // Start timer query
    glBeginQuery(GL_TIME_ELAPSED, queryID);
#endif // ENABLE_RENDERER_INFO


    // Draw
    glBindVertexArray(meshVAO);
    glBindBuffer(GL_DRAW_INDIRECT_BUFFER, IndirectCommandBuffer);

    if (indirectCommands.size() > 1)
    {
        glMultiDrawElementsIndirect(GL_TRIANGLES, GL_UNSIGNED_INT, nullptr, static_cast<GLsizei>(indirectCommands.size()), 0);
    }
    else
    {
        glDrawElementsIndirect(GL_TRIANGLES, GL_UNSIGNED_INT, nullptr);
    }

    // Unbind
    glBindBuffer(GL_DRAW_INDIRECT_BUFFER, 0);
    glBindVertexArray(0);

#ifdef ENABLE_RENDERER_INFO
    // End timer query
    glEndQuery(GL_TIME_ELAPSED);

    // Get result (blocking)
    GLuint64 timeElapsed = 0;
    glGetQueryObjectui64v(queryID, GL_QUERY_RESULT, &timeElapsed);

    glDeleteQueries(1, &queryID);

    // Convert from nanoseconds to milliseconds
    double gpuTimeMs = timeElapsed / 1e6;
    std::cout << "[GPU] GLTFMESHRender took: " << gpuTimeMs << " ms" << std::endl;
#endif // ENABLE_RENDERER_INFO
}

void GLTFMESHRenderer::uploadBuffersIfRequired()
{
    if (!IsBuffersUpdateRequired) return;

    // Ensure arrays have consistent sizes (positions, normals, texcoords per vertex)
    size_t vertexCount = cpuPositions.size() / 3;
    // If normals are missing, expand to zeros
    if (cpuNormals.size() / 3 < vertexCount)
    {
        cpuNormals.resize(vertexCount * 3, 0.0f);
    }
    // If texcoords missing, expand
    if (cpuTexcoords.size() / 2 < vertexCount)
    {
        cpuTexcoords.resize(vertexCount * 2, 0.0f);
    }

    // Upload positions (floats)
    if (!cpuPositions.empty())
    {
        glNamedBufferSubData(meshPosVBO, 0, cpuPositions.size() * sizeof(float), cpuPositions.data());
    }

    // Upload normals
    if (!cpuNormals.empty())
    {
        glNamedBufferSubData(meshNormVBO, 0, cpuNormals.size() * sizeof(float), cpuNormals.data());
    }

    // Upload texcoords
    if (!cpuTexcoords.empty())
    {
        glNamedBufferSubData(meshTexVBO, 0, cpuTexcoords.size() * sizeof(float), cpuTexcoords.data());
    }

    // Upload indices (we store uint32 on GPU)
    if (!cpuIndices.empty())
    {
        glNamedBufferSubData(meshEBO, 0, cpuIndices.size() * sizeof(uint32_t), cpuIndices.data());
    }

    IsBuffersUpdateRequired = false;
}

// Helper to copy accessor float data (expectedNumComponents: 2 for texcoord, 3 for position/normal)
void GLTFMESHRenderer::copyAccessorToFloatVector(const tinygltf::Model& model, const tinygltf::Accessor& accessor, std::vector<float>& out, int expectedNumComponents)
{
    const tinygltf::BufferView& bv = model.bufferViews[accessor.bufferView];
    const tinygltf::Buffer& buff = model.buffers[bv.buffer];
    const size_t accessorOffset = (bv.byteOffset ? bv.byteOffset : 0) + (accessor.byteOffset ? accessor.byteOffset : 0);
    const unsigned char* dataPtr = buff.data.data() + accessorOffset;

    // Number of elements (e.g., vertices)
    size_t count = accessor.count;

    // componentType should be float for glTF POSITION/NORMAL/TEXCOORD
    if (accessor.componentType != TINYGLTF_COMPONENT_TYPE_FLOAT)
    {
        throw std::runtime_error("[GLTFMESHRenderer] Unsupported accessor componentType (expected FLOAT). Consider supporting normalized integer formats.");
    }

    // stride handling: if bufferView.byteStride == 0 then tightly packed
    size_t stride = bv.byteStride ? bv.byteStride : (expectedNumComponents * sizeof(float));

    out.reserve(out.size() + count * expectedNumComponents);

    for (size_t i = 0; i < count; ++i)
    {
        const float* elemPtr = reinterpret_cast<const float*>(dataPtr + i * stride);
        for (int c = 0; c < expectedNumComponents; ++c)
        {
            out.push_back(elemPtr[c]);
        }
    }
}

// Helper to copy indices to uint32 vector (handles BYTE/USHORT/UINT)
void GLTFMESHRenderer::copyAccessorToIndexVector(const tinygltf::Model& model, const tinygltf::Accessor& accessor, std::vector<uint32_t>& out, uint32_t indexOffset)
{
    const tinygltf::BufferView& bv = model.bufferViews[accessor.bufferView];
    const tinygltf::Buffer& buff = model.buffers[bv.buffer];
    const size_t accessorOffset = (bv.byteOffset ? bv.byteOffset : 0) + (accessor.byteOffset ? accessor.byteOffset : 0);
    const unsigned char* dataPtr = buff.data.data() + accessorOffset;
    size_t count = accessor.count;

    out.reserve(out.size() + count);

    switch (accessor.componentType)
    {
    case TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT:
    {
        const uint32_t* p = reinterpret_cast<const uint32_t*>(dataPtr);
        for (size_t i = 0; i < count; ++i) out.push_back(static_cast<uint32_t>(p[i]));
        break;
    }
    case TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT:
    {
        const uint16_t* p = reinterpret_cast<const uint16_t*>(dataPtr);
        for (size_t i = 0; i < count; ++i) out.push_back(static_cast<uint32_t>(p[i]));
        break;
    }
    case TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE:
    {
        const uint8_t* p = reinterpret_cast<const uint8_t*>(dataPtr);
        for (size_t i = 0; i < count; ++i) out.push_back(static_cast<uint32_t>(p[i]));
        break;
    }
    default:
        throw std::runtime_error("[GLTFMESHRenderer] Unsupported index component type");
    }
}

void GLTFMESHRenderer::ExperimentalHelper()
{
    if (meshStructureForRendering.empty()) return;

    // First upload buffers if data changed
    uploadBuffersIfRequired();

    // Build indirect commands and a consolidated orientation array
    indirectCommands.clear();
    std::vector<GLTFPrimitivesOrientation> allOrientations;
    allOrientations.reserve(10000);

    GLuint baseInstance = 0;
    for (auto const& kv : meshStructureForRendering)
    {
        const std::string& meshKey = kv.first;
        const MeshStructureForRendering& msr = kv.second;

        // Append orientations for this mesh
        auto it = primitivesOrientationPerMesh.find(meshKey);
        size_t instancesForThisMesh = 0;
        if (it != primitivesOrientationPerMesh.end())
        {
            instancesForThisMesh = it->second.size();
            allOrientations.insert(allOrientations.end(), it->second.begin(), it->second.end());
        }
        else
        {
            instancesForThisMesh = static_cast<size_t>(msr.meshInstances);
            // push default orientations if none present
            for (size_t i = 0; i < instancesForThisMesh; ++i)
                allOrientations.emplace_back(glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(1.0f), -1);
        }

        // create an indirect command per mesh
        DrawElementsIndirectCommand cmd(
            static_cast<GLuint>(msr.meshIndexCnt),               // count
            static_cast<GLuint>(instancesForThisMesh),           // primCount (instances)
            static_cast<GLuint>(msr.meshIndexBufferOffset),      // firstIndex (index offset in elements)
            static_cast<GLuint>(msr.meshPositionVertexOffset),   // baseVertex
            //0,
            baseInstance                                         // baseInstance
        );
        indirectCommands.push_back(cmd);

        baseInstance += static_cast<GLuint>(instancesForThisMesh);
    }

    // Upload all orientations to SSBO
    if (!allOrientations.empty())
    {
        glNamedBufferSubData(OrientationSSBO, 0, allOrientations.size() * sizeof(GLTFPrimitivesOrientation), allOrientations.data());
    }

    // Uploading the material struct Here! Has to be done.
    std::vector<GLTFMaterial> materialsForThisInstance;
    for (const auto& material : gltfMaterialMapping)
    {
        materialsForThisInstance.push_back(material.second);
    }
    if (!materialsForThisInstance.empty())
    {
        glNamedBufferSubData(MaterialSSBO, 0
            , materialsForThisInstance.size() * sizeof(GLTFMaterial), materialsForThisInstance.data());
    }

    // Upload indirect commands
    if (!indirectCommands.empty())
    {
        glNamedBufferSubData(IndirectCommandBuffer, 0, indirectCommands.size() * sizeof(DrawElementsIndirectCommand), indirectCommands.data());
    }
}
