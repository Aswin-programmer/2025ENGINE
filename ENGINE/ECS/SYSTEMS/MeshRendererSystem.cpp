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
    lightingQuery = ecsWorld->query_builder<LightingComponent>()
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
                    std::vector<glm::mat4>(MAX_JOINTS, glm::mat4(1.f))
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
                    jointMatrices
                );
            }
            catch (const std::exception& ex) {
                std::cerr << "Error processing animated mesh for entity " << e.name() << ": " << ex.what() << std::endl;
            }
        });

    lightingQuery.each(
        [this](flecs::entity e,const LightingComponent& lightingComponent)
        {
            try
            {
                gltfMeshRenderer->AddLightToTheRenderer(
                    GLTFLight(
                        lightingComponent.lightType,
                        lightingComponent.position,
                        lightingComponent.lightColor,
                        lightingComponent.ambientStrength,
                        lightingComponent.diffuseStrength,
                        lightingComponent.specularStrength
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

void MeshRendererSystem::DebugMenu(mu_Context* ctx)
{
    if (!ecsWorld) return;

    if (mu_begin_window(ctx, "Entity Debug Menu", mu_rect(10, 10, 400, 600)))
    {
        // Static Mesh Entities
        if (mu_header(ctx, "Static Mesh Entities"))
        {
            staticMeshRendererQuery.each([ctx](flecs::entity e, TransfromComponent& transform, MeshComponent& mesh) {
                if (mu_begin_treenode(ctx, e.name()))
                {
                    char buf[64];

                    // Mesh Name
                    sprintf(buf, "Mesh: %s", mesh.GetMeshName().c_str());
                    mu_label(ctx, buf);

                    // Position
                    int widths[] = { 50, -1 };
                    mu_layout_row(ctx, 2, widths, 0);
                    mu_label(ctx, "Pos X:"); mu_slider(ctx, &transform.Position.x, -100.f, 100.f);
                    mu_label(ctx, "Pos Y:"); mu_slider(ctx, &transform.Position.y, -100.f, 100.f);
                    mu_label(ctx, "Pos Z:"); mu_slider(ctx, &transform.Position.z, -100.f, 100.f);

                    // Rotation
                    mu_label(ctx, "Rotation X:"); mu_slider(ctx, &transform.Rotation.x, -180.f, 180.f);
                    mu_label(ctx, "Rotation Y:"); mu_slider(ctx, &transform.Rotation.y, -180.f, 180.f);
                    mu_label(ctx, "Rotation Z:"); mu_slider(ctx, &transform.Rotation.z, -180.f, 180.f);

                    // Scale
                    mu_label(ctx, "Scale X:"); mu_slider(ctx, &transform.Scale.x, 0.f, 10.f);
                    mu_label(ctx, "Scale Y:"); mu_slider(ctx, &transform.Scale.y, 0.f, 10.f);
                    mu_label(ctx, "Scale Z:"); mu_slider(ctx, &transform.Scale.z, 0.f, 10.f);

                    mu_end_treenode(ctx);
                }
                });
        }

        // Animated Mesh Entities
        if (mu_header(ctx, "Animated Mesh Entities"))
        {
            animatedMeshRendererQuery.each([ctx](flecs::entity e, TransfromComponent& transform, MeshComponent& mesh, AnimationComponent& anim) {
                if (mu_begin_treenode(ctx, e.name()))
                {
                    char buf[64];

                    // Mesh Name
                    sprintf(buf, "Mesh: %s", mesh.GetMeshName().c_str());
                    mu_label(ctx, buf);

                    // Animation info
                    sprintf(buf, "Anim Time: %.2f", anim.GetCurrentOnTime());
                    mu_label(ctx, buf);

                    // Transform sliders
                    int widths[] = { 50, -1 };
                    mu_layout_row(ctx, 2, widths, 0);
                    mu_label(ctx, "Pos X:"); mu_slider(ctx, &transform.Position.x, -100.f, 100.f);
                    mu_label(ctx, "Pos Y:"); mu_slider(ctx, &transform.Position.y, -100.f, 100.f);
                    mu_label(ctx, "Pos Z:"); mu_slider(ctx, &transform.Position.z, -100.f, 100.f);

                    mu_label(ctx, "Rot X:"); mu_slider(ctx, &transform.Rotation.x, -180.f, 180.f);
                    mu_label(ctx, "Rot Y:"); mu_slider(ctx, &transform.Rotation.y, -180.f, 180.f);
                    mu_label(ctx, "Rot Z:"); mu_slider(ctx, &transform.Rotation.z, -180.f, 180.f);

                    mu_label(ctx, "Scale X:"); mu_slider(ctx, &transform.Scale.x, 0.f, 10.f);
                    mu_label(ctx, "Scale Y:"); mu_slider(ctx, &transform.Scale.y, 0.f, 10.f);
                    mu_label(ctx, "Scale Z:"); mu_slider(ctx, &transform.Scale.z, 0.f, 10.f);

                    mu_end_treenode(ctx);
                }
                });
        }

        // Lighting Entities
        if (mu_header(ctx, "Lighting Entities"))
        {
            lightingQuery.each([ctx](flecs::entity e, LightingComponent& light)
                {
                    if (mu_begin_treenode(ctx, e.name()))
                    {
                        char buf[64];

                        // Light Type
                        sprintf(buf, "Type: %s",
                            light.lightType == GLTFLightType::Directional ? "Directional" : "Ending");
                        mu_label(ctx, buf);

                        // --- Light Type Toggle ---
                        int widths[] = { 80, -1 };
                        mu_layout_row(ctx, 2, widths, 0);

                        mu_label(ctx, "Directional:");
                        int typeValue = (int)light.lightType;

                        if (mu_checkbox(ctx, "##lightType", &typeValue)) {
                            // Clamp to valid enum range
                            if (typeValue < 0) typeValue = 0;
                            if (typeValue > 1) typeValue = 1;

                            light.lightType = (GLTFLightType)typeValue;
                        }

                        // --- Position sliders ---
                        mu_label(ctx, "Pos X:"); mu_slider(ctx, &light.position.x, -100.f, 100.f);
                        mu_label(ctx, "Pos Y:"); mu_slider(ctx, &light.position.y, -100.f, 100.f);
                        mu_label(ctx, "Pos Z:"); mu_slider(ctx, &light.position.z, -100.f, 100.f);

                        // --- Color sliders ---
                        mu_label(ctx, "Color R:"); mu_slider(ctx, &light.lightColor.r, 0.f, 1.f);
                        mu_label(ctx, "Color G:"); mu_slider(ctx, &light.lightColor.g, 0.f, 1.f);
                        mu_label(ctx, "Color B:"); mu_slider(ctx, &light.lightColor.b, 0.f, 1.f);

                        // --- Strength sliders ---
                        mu_label(ctx, "Ambient:");  mu_slider(ctx, &light.ambientStrength, 0.f, 5.f);
                        mu_label(ctx, "Diffuse:");  mu_slider(ctx, &light.diffuseStrength, 0.f, 5.f);
                        mu_label(ctx, "Specular:"); mu_slider(ctx, &light.specularStrength, 0.f, 5.f);

                        mu_end_treenode(ctx);
                    }
                });
        }

        mu_end_window(ctx);
    }
}

