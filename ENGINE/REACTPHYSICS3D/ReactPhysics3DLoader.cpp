#include "ReactPhysics3DLoader.h"

std::unordered_map<std::string, ReactPhysics3DBoxColloider> ReactPhysics3DLoader::reactPhysics3dBoxColloiderContainer;

bool ReactPhysics3DLoader::LoadReactPhysics3DBoxColloiderFromGLTFModel(
	std::string& fileName, 
	tinygltf::Model& model
)
{
    // Validate scene
    if (model.scenes.empty() || model.defaultScene < 0 || model.defaultScene >= static_cast<int>(model.scenes.size()))
    {
        std::cout << "[ReactPhysics3DLoader] Model has no valid default scene: " << fileName << "\n";
        return false;
    }

    const tinygltf::Scene& scene = model.scenes[model.defaultScene];

    for (int nodeIdx : scene.nodes)
    {
        const tinygltf::Node& node = model.nodes[nodeIdx];
        if (node.name == "BoxColloider")
        {
            if (node.mesh >= 0)
            {
                const tinygltf::Mesh& mesh = model.meshes[node.mesh];
                for (const auto& primitive : mesh.primitives)
                {
                    // Searching for the position primitive
                    if (primitive.attributes.find("POSITION") != primitive.attributes.end())
                    {
                        const tinygltf::Accessor& posAccessor = model.accessors[primitive.attributes.at("POSITION")];
                        const tinygltf::BufferView& bufferView = model.bufferViews[posAccessor.bufferView];
                        const tinygltf::Buffer& buffer = model.buffers[bufferView.buffer];
                        const size_t accessorOffset = (bufferView.byteOffset ? bufferView.byteOffset : 0)
                            + (posAccessor.byteOffset ? posAccessor.byteOffset : 0);
                        const unsigned char* dataPtr = buffer.data.data() + accessorOffset;

                        // Number of vertices:
                        size_t verticesCount = posAccessor.count;

                        if (posAccessor.componentType != TINYGLTF_COMPONENT_TYPE_FLOAT)
                        {
                            throw std::runtime_error("[ReactPhysics3DLoader] Position Attribute is not float!.");
                        }
                        if (posAccessor.type != TINYGLTF_TYPE_VEC3)
                        {
                            throw std::runtime_error("[ReactPhysics3DLoader] POSITION is not VEC3");
                        }

                        size_t stride = bufferView.byteStride ? bufferView.byteStride : (3 * sizeof(float));

                        float minX = std::numeric_limits<float>::max();
                        float minY = std::numeric_limits<float>::max();
                        float minZ = std::numeric_limits<float>::max();

                        float maxX = -std::numeric_limits<float>::max();
                        float maxY = -std::numeric_limits<float>::max();
                        float maxZ = -std::numeric_limits<float>::max();

                        glm::vec3 localPos(0.0f);
                        if (node.translation.size() == 3)
                        {
                            localPos = {
                                node.translation[0],
                                node.translation[1],
                                node.translation[2]
                            };
                        }

                        glm::quat localRot(1, 0, 0, 0); // identity
                        if (node.rotation.size() == 4)
                        {
                            localRot = glm::quat(
                                node.rotation[3], // w
                                node.rotation[0], // x
                                node.rotation[1], // y
                                node.rotation[2]  // z
                            );
                        }

                        glm::vec3 localScale(1.0f);
                        if (node.scale.size() == 3)
                        {
                            localScale = {
                                node.scale[0],
                                node.scale[1],
                                node.scale[2]
                            };
                        }

                        for (size_t i = 0; i < verticesCount; i++)
                        {
                            const float* elemPtr = reinterpret_cast<const float*>(dataPtr + i * stride);

                            const float x = elemPtr[0] * localScale.x;
                            const float y = elemPtr[1] * localScale.y;
                            const float z = elemPtr[2] * localScale.z;

                            minX = std::min(minX, x);
                            maxX = std::max(maxX, x);

                            minY = std::min(minY, y);
                            maxY = std::max(maxY, y);

                            minZ = std::min(minZ, z);
                            maxZ = std::max(maxZ, z);
                        }

                        std::cout << fileName << " : " << minX << " " << maxX << " " << minY << " " << maxY << " " << minZ << " " << maxZ << " " << std::endl;

                        reactPhysics3dBoxColloiderContainer.emplace(
                            fileName,
                            ReactPhysics3DBoxColloider(
                                localPos,
                                localRot,
                                glm::vec3(
                                    (maxX - minX) * 0.5,
                                    (maxY - minY) * 0.5,
                                    (maxZ - minZ) * 0.5
                                )
                            )
                        );
                        return true;
                    }
                    else
                    {
                        std::cout << "[ReactPhysics3DLoader] " << fileName << " : Model mesh doesn't have POSITION attribute!" << std::endl;
                        return false;
                    }
                }
            }
            else
            {
                std::cout << "[ReactPhysics3DLoader] " << fileName << " : Model has no valid mesh!." << std::endl;
                return false;
            }
        }
    }
    return false;
}

ReactPhysics3DBoxColloider& ReactPhysics3DLoader::GetReactPhysics3DBoxColloiderFromGLTFModel(std::string& fileName)
{
    auto it = reactPhysics3dBoxColloiderContainer.find(fileName);
    if (it == reactPhysics3dBoxColloiderContainer.end()) {
        throw std::runtime_error("Collider not found for GLTF model");
    }
    return it->second;
}

void ReactPhysics3DLoader::DeleteAllReactPhysics3DColloiders()
{
    std::cout << "Deleting all the ReactPhysics3DColloiders!" << std::endl;
    reactPhysics3dBoxColloiderContainer.clear();
    std::cout << "Finished Deleting all the ReactPhysics3DColloiders!" << std::endl;
}
