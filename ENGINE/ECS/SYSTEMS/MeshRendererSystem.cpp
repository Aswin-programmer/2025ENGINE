#include "MeshRendererSystem.h"

MeshRendererSystem::MeshRendererSystem()
{
}

MeshRendererSystem::MeshRendererSystem(std::string vertPath, std::string fragPath, std::shared_ptr<flecs::world> ecsWorld_)
    : vertexShaderPath(vertPath),
    fragmentShaderPath(fragPath),
    gltfMeshRenderer(std::make_shared<GLTFMESHRenderer>()),
    meshRendererShader(std::make_shared<Shader>(vertPath.c_str(), fragPath.c_str())),
    ecsWorld(ecsWorld_)
{
}

MeshRendererSystem::~MeshRendererSystem()
{
}

void MeshRendererSystem::InitMeshRendererSystem()
{
    if (!ecsWorld) {
        std::cerr << "ECS World is null in InitMeshRendererSystem!" << std::endl;
        return;
    }

    meshRendererShader->SetupDefaultTextureSlots();

    // Build static mesh query (entities with Transform and Mesh but NO Animation)
    staticMeshRendererQuery = ecsWorld->query_builder<TransfromComponent, MeshComponent>()
        .without<AnimationComponent>()
        .build();

    // Build animated mesh query (entities with Transform, Mesh AND Animation)
    animatedMeshRendererQuery = ecsWorld->query_builder<TransfromComponent, MeshComponent, AnimationComponent>()
        .build();

    // Build lighting query
    lightingQuery = ecsWorld->query_builder<TransfromComponent, LightingComponent>()
        .build();

    std::cout << "MeshRendererSystem initialized successfully!" << std::endl;
}

void MeshRendererSystem::MeshRendererUpdate(glm::mat4 view, glm::mat4 proj, glm::vec3 cameraPos)
{
    if (!ecsWorld) {
        std::cerr << "ECS World is null in MeshRendererUpdate!" << std::endl;
        return;
    }

    if (!meshRendererShader) {
        std::cerr << "Shader is null in MeshRendererUpdate!" << std::endl;
        return;
    }

    if (!gltfMeshRenderer) {
        std::cerr << "GLTF Mesh Renderer is null in MeshRendererUpdate!" << std::endl;
        return;
    }

    // Clear previous frame's models
    gltfMeshRenderer->CleanUp();

    // Process static meshes (without animation)
    staticMeshRendererQuery.each([this](flecs::entity e, TransfromComponent& transform, MeshComponent& mesh)
        {
            try {
                gltfMeshRenderer->AddGLTFModelToRenderer(
                    mesh.GetMeshName(),
                    GLTFModelOrientation(
                        transform.GetPosition(),
                        transform.GetRotation(),
                        transform.GetScale(),
                        -1, -1
                    ),
                    true,
                    std::vector<glm::mat4>(MAX_JOINTS, glm::mat4(1.f)),
                    mesh.ambientStrength,
                    mesh.diffuseStrength,
                    mesh.specularStrength
                );
            }
            catch (const std::exception& ex) {
                std::cerr << "Error processing static mesh for entity " << e.name() << ": " << ex.what() << std::endl;
            }
        });

    // Process animated meshes (with animation)
    animatedMeshRendererQuery.each([this](flecs::entity e, TransfromComponent& transform, MeshComponent& mesh, AnimationComponent& anim)
        {
            try {
                auto jointMatrices = GLTFMESHSkeletalAnimationLoader::GetGLTFMESHSkeletalAnimations(
                    mesh.GetStrippedMeshName()
                ).getJointMatrices(anim.GetCurrentOnTime());

                gltfMeshRenderer->AddGLTFModelToRenderer(
                    mesh.GetMeshName(),
                    GLTFModelOrientation(
                        transform.GetPosition(),
                        transform.GetRotation(),
                        transform.GetScale(),
                        -1, -1
                    ),
                    true,
                    jointMatrices,
                    mesh.ambientStrength,
                    mesh.diffuseStrength,
                    mesh.specularStrength
                );
            }
            catch (const std::exception& ex) {
                std::cerr << "Error processing animated mesh for entity " << e.name() << ": " << ex.what() << std::endl;
            }
        });

    lightingQuery.each(
        [this](flecs::entity e, const TransfromComponent& transfromComponent,const LightingComponent& lightingComponent)
        {
            try
            {
                gltfMeshRenderer->AddLightToTheRenderer(
                    GLTFLight(
                        lightingComponent.lightType,
                        transfromComponent.Position,
                        lightingComponent.lightColor,
                        lightingComponent.ambientStrength,
                        lightingComponent.diffuseStrength,
                        lightingComponent.specularStrength,
                        lightingComponent.constant,
                        lightingComponent.linear,
                        lightingComponent.quadratic,
                        lightingComponent.direction,
                        lightingComponent.cutOff
                    )
                ); 
            }
            catch (const std::exception& ex)
            {
                std::cerr << "Error processing lighting for entity " << e.name() << ": " << ex.what() << std::endl;
            }
        }
    ); 



    // Render all collected models
    gltfMeshRenderer->ExperimentalHelper();
    meshRendererShader->use();
    meshRendererShader->setMat4("view", view);
    meshRendererShader->setMat4("projection", proj);
    meshRendererShader->setVec3("viewPos", cameraPos);
    gltfMeshRenderer->GLTFMESHRender();
}

