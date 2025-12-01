#include "DebugMenuUISystem.h"

DebugMenuUISystem::DebugMenuUISystem(mu_Context* ctx, std::shared_ptr<flecs::world> ecsWorld)
	:
	ctx{ctx},
	ecsWorld{ecsWorld}
{
}

DebugMenuUISystem::~DebugMenuUISystem()
{

}

void DebugMenuUISystem::InitDebugMenuUISystem()
{
	if (!ecsWorld) {
		std::cerr << "ECS World is null in InitMeshRendererSystem!" << std::endl;
		return;
	}

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

    std::cout << "DebugMenuUISystem initialized successfully!" << std::endl;
}

void DebugMenuUISystem::StartRenderMenuUISystem()
{
    ctx->style->colors[MU_COLOR_WINDOWBG] = mu_color(0, 0, 0, 0); // fully transparent
    ctx->style->colors[MU_COLOR_PANELBG] = mu_color(0, 0, 0, 0);  // transparent panels

    mu_begin(ctx);
}

void DebugMenuUISystem::RenderUIMenu()
{
    if (!ecsWorld) return;

    if (mu_begin_window(ctx, "Entity Debug Menu", mu_rect(10, 10, 400, 600)))
    {
        // Static Mesh Entities
        if (mu_header(ctx, "Static Mesh Entities"))
        {
            staticMeshRendererQuery.each([this](flecs::entity e, TransfromComponent& transform, MeshComponent& mesh) {
                if (mu_begin_treenode(ctx, e.name()))
                {
                    // Setting up custom max
                    static int myMaxMin = 100;
                    static char myMinMinBuffer[64] = "100";
                    mu_label(ctx, "Set Custom MaxMin : ");
                    // Draw the textbox. MU_RES_SUBMIT is returned when the user hits Enter or clicks away.
                    if (mu_textbox(ctx, myMinMinBuffer, sizeof(myMinMinBuffer)) & MU_RES_SUBMIT) {
                        int temp;
                        if (sscanf(myMinMinBuffer, "%d", &temp) == 1) {
                            myMaxMin = temp;
                            std::cout << "Changed MinMax Range to " << myMaxMin << std::endl;
                        }
                        else {
                            sprintf(myMinMinBuffer, "%d", myMaxMin);
                        }
                    }

                    char buf[64];

                    // Mesh Name
                    sprintf(buf, "Mesh: %s", mesh.GetMeshName().c_str());
                    mu_label(ctx, buf);

                    // Position
                    int widths[] = { 50, -1 };
                    mu_layout_row(ctx, 2, widths, 0);
                    // --- Position sliders ---
                    mu_label(ctx, "Pos X:"); mu_slider(ctx, &transform.Position.x, -(myMaxMin + fabsf(transform.Position.x)), myMaxMin + fabsf(transform.Position.x));
                    mu_label(ctx, "Pos Y:"); mu_slider(ctx, &transform.Position.y, -(myMaxMin + fabsf(transform.Position.y)), myMaxMin + fabsf(transform.Position.y));
                    mu_label(ctx, "Pos Z:"); mu_slider(ctx, &transform.Position.z, -(myMaxMin + fabsf(transform.Position.z)), myMaxMin + fabsf(transform.Position.z));

                    // --- Rotation sliders ---
                    mu_label(ctx, "Rot X:"); mu_slider(ctx, &transform.Rotation.x, -(myMaxMin + fabsf(transform.Rotation.x)), myMaxMin + fabsf(transform.Rotation.x));
                    mu_label(ctx, "Rot Y:"); mu_slider(ctx, &transform.Rotation.y, -(myMaxMin + fabsf(transform.Rotation.y)), myMaxMin + fabsf(transform.Rotation.y));
                    mu_label(ctx, "Rot Z:"); mu_slider(ctx, &transform.Rotation.z, -(myMaxMin + fabsf(transform.Rotation.z)), myMaxMin + fabsf(transform.Rotation.z));

                    // --- Scale sliders ---
                    mu_label(ctx, "Scale X:"); mu_slider(ctx, &transform.Scale.x, -(myMaxMin + fabsf(transform.Scale.x)), myMaxMin + fabsf(transform.Scale.x));
                    mu_label(ctx, "Scale Y:"); mu_slider(ctx, &transform.Scale.y, -(myMaxMin + fabsf(transform.Scale.y)), myMaxMin + fabsf(transform.Scale.y));
                    mu_label(ctx, "Scale Z:"); mu_slider(ctx, &transform.Scale.z, -(myMaxMin + fabsf(transform.Scale.z)), myMaxMin + fabsf(transform.Scale.z));

                    mu_end_treenode(ctx);
                }
                });
        }

        // Animated Mesh Entities
        if (mu_header(ctx, "Animated Mesh Entities"))
        {
            animatedMeshRendererQuery.each([this](flecs::entity e, TransfromComponent& transform, MeshComponent& mesh, AnimationComponent& anim) {
                if (mu_begin_treenode(ctx, e.name()))
                {
                    // Setting up custom max
                    static int myMaxMin = 100;
                    static char myMinMinBuffer[64] = "100";
                    mu_label(ctx, "Set Custom MaxMin : ");
                    // Draw the textbox. MU_RES_SUBMIT is returned when the user hits Enter or clicks away.
                    if (mu_textbox(ctx, myMinMinBuffer, sizeof(myMinMinBuffer)) & MU_RES_SUBMIT) {
                        int temp;
                        if (sscanf(myMinMinBuffer, "%d", &temp) == 1) {
                            myMaxMin = temp;
                            std::cout << "Changed MinMax Range to " << myMaxMin << std::endl;
                        }
                        else {
                            sprintf(myMinMinBuffer, "%d", myMaxMin);
                        }
                    }

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
                    // --- Position sliders ---
                    mu_label(ctx, "Pos X:"); mu_slider(ctx, &transform.Position.x, -(myMaxMin + fabsf(transform.Position.x)), myMaxMin + fabsf(transform.Position.x));
                    mu_label(ctx, "Pos Y:"); mu_slider(ctx, &transform.Position.y, -(myMaxMin + fabsf(transform.Position.y)), myMaxMin + fabsf(transform.Position.y));
                    mu_label(ctx, "Pos Z:"); mu_slider(ctx, &transform.Position.z, -(myMaxMin + fabsf(transform.Position.z)), myMaxMin + fabsf(transform.Position.z));

                    // --- Rotation sliders ---
                    mu_label(ctx, "Rot X:"); mu_slider(ctx, &transform.Rotation.x, -(myMaxMin + fabsf(transform.Rotation.x)), myMaxMin + fabsf(transform.Rotation.x));
                    mu_label(ctx, "Rot Y:"); mu_slider(ctx, &transform.Rotation.y, -(myMaxMin + fabsf(transform.Rotation.y)), myMaxMin + fabsf(transform.Rotation.y));
                    mu_label(ctx, "Rot Z:"); mu_slider(ctx, &transform.Rotation.z, -(myMaxMin + fabsf(transform.Rotation.z)), myMaxMin + fabsf(transform.Rotation.z));

                    // --- Scale sliders ---
                    mu_label(ctx, "Scale X:"); mu_slider(ctx, &transform.Scale.x, -(myMaxMin + fabsf(transform.Scale.x)), myMaxMin + fabsf(transform.Scale.x));
                    mu_label(ctx, "Scale Y:"); mu_slider(ctx, &transform.Scale.y, -(myMaxMin + fabsf(transform.Scale.y)), myMaxMin + fabsf(transform.Scale.y));
                    mu_label(ctx, "Scale Z:"); mu_slider(ctx, &transform.Scale.z, -(myMaxMin + fabsf(transform.Scale.z)), myMaxMin + fabsf(transform.Scale.z));

                    mu_end_treenode(ctx);
                }
                });
        }

        // Lighting Entities
        if (mu_header(ctx, "Lighting Entities"))
        {
            lightingQuery.each([this](flecs::entity e, LightingComponent& light)
                {
                    if (mu_begin_treenode(ctx, e.name()))
                    {
                        // Setting up custom max
                        static int myMaxMin = 100;
                        static char myMinMinBuffer[64] = "100";
                        mu_label(ctx, "Set Custom MaxMin : ");
                        // Draw the textbox. MU_RES_SUBMIT is returned when the user hits Enter or clicks away.
                        if (mu_textbox(ctx, myMinMinBuffer, sizeof(myMinMinBuffer)) & MU_RES_SUBMIT) {
                            int temp;
                            if (sscanf(myMinMinBuffer, "%d", &temp) == 1) {
                                myMaxMin = temp;
                                std::cout << "Changed MinMax Range to " << myMaxMin << std::endl;
                            }
                            else {
                                sprintf(myMinMinBuffer, "%d", myMaxMin);
                            }
                        }

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
                        mu_label(ctx, "Pos X:"); mu_slider(ctx, &light.position.x, -(myMaxMin+fabsf(light.position.x)), myMaxMin+fabsf(light.position.x));
                        mu_label(ctx, "Pos Y:"); mu_slider(ctx, &light.position.y, -(myMaxMin+fabsf(light.position.y)), myMaxMin+fabsf(light.position.y));
                        mu_label(ctx, "Pos Z:"); mu_slider(ctx, &light.position.z, -(myMaxMin+fabsf(light.position.z)), myMaxMin+fabsf(light.position.z));

                        // --- Color sliders ---
                        mu_label(ctx, "Color R:"); mu_slider(ctx, &light.lightColor.r, 0, myMaxMin+fabsf(light.lightColor.r));
                        mu_label(ctx, "Color G:"); mu_slider(ctx, &light.lightColor.g, 0, myMaxMin+fabsf(light.lightColor.r));
                        mu_label(ctx, "Color B:"); mu_slider(ctx, &light.lightColor.b, 0, myMaxMin+fabsf(light.lightColor.r));

                        // --- Strength sliders ---
                        mu_label(ctx, "Ambient:");  mu_slider(ctx, &light.ambientStrength, -(myMaxMin+fabsf(light.ambientStrength)), myMaxMin+fabsf(light.ambientStrength));
                        mu_label(ctx, "Diffuse:");  mu_slider(ctx, &light.diffuseStrength, -(myMaxMin+fabsf(light.diffuseStrength)), myMaxMin+fabsf(light.diffuseStrength));
                        mu_label(ctx, "Specular:"); mu_slider(ctx, &light.specularStrength,-(myMaxMin+fabsf(light.specularStrength)),myMaxMin+fabsf(light.specularStrength));

                        mu_end_treenode(ctx);
                    }
                });
        }

        mu_end_window(ctx);
    }
}

void DebugMenuUISystem::PerformanceUIMenu()
{
    if (mu_begin_window(ctx, "Performance & Debug Info", mu_rect(460, 10, 300, 200)))
    {
        // Display FPS
        float fps = Window::GetFPSValue();
        char buf[64];
        sprintf(buf, "FPS: %.1f", fps);
        mu_label(ctx, buf);

        // Display Frame Time
        sprintf(buf, "Frame Time: %.2f ms", Window::getdt() * 1000.f);
        mu_label(ctx, buf);

        // Window size
        sprintf(buf, "Window Size: %u x %u", Window::getWidth(), Window::getHeight());
        mu_label(ctx, buf);

        // Background Color Preview
        mu_label(ctx, "Background Color:");
        int widths[] = { 20, -1 };
        mu_layout_row(ctx, 2, widths, 0);

        mu_label(ctx, "R:"); mu_slider(ctx, &Window_r, 0.0f, 1.0f);
        mu_label(ctx, "G:"); mu_slider(ctx, &Window_g, 0.0f, 1.0f);
        mu_label(ctx, "B:"); mu_slider(ctx, &Window_b, 0.0f, 1.0f);
        mu_label(ctx, "A:"); mu_slider(ctx, &Window_a, 0.0f, 1.0f);

        mu_end_window(ctx);
    }
}

void DebugMenuUISystem::EntityManagerMenu()
{
    if (mu_begin_window(ctx, "Entity Manager", mu_rect(800, 10, 350, 500)))
    {
        static char entityName[64] = "NewEntity";
        static char meshName[64] = "NewMesh";
        static char lightingType[64] = "NoLighting";
        static int isTranformCompontentPresent = false;
        static int isMeshComponentPresent = false;
        static int isAnimationComponentPresent = false;
        static int isLightingComponentPresent = false;

        int widths[] = { 100, -1 };
        mu_layout_row(ctx, 2, widths, 0);
        mu_label(ctx, "Entity Name:");
        mu_textbox(ctx, entityName, sizeof(entityName));

        mu_label(ctx, "Mesh Name:");
        mu_textbox(ctx, meshName, sizeof(meshName));

        mu_label(ctx, "Lighting Type: ");
        mu_textbox(ctx, lightingType, sizeof(lightingType));


        // --- Switch back to single-column layout ---
        mu_layout_row(ctx, 1, nullptr, 0);   // 1 column = full-width items

        // Now everything becomes vertical (top-to-bottom)
        mu_label(ctx, "Components:");
        mu_checkbox(ctx, "Transform Component", &isTranformCompontentPresent);
        mu_checkbox(ctx, "Mesh Component", &isMeshComponentPresent);
        mu_checkbox(ctx, "Animation Component", &isAnimationComponentPresent);
        mu_checkbox(ctx, "Lighting Component", &isLightingComponentPresent);

        // Creating the Entity
        if (mu_button(ctx, "CREATE ENTITY"))
        {
            ecsWorld->defer_begin();             // <-- Safe region

            flecs::entity e = ecsWorld->lookup(entityName);

            if (!e.is_valid())
            {
                // Now safe to create entity
                flecs::entity newE = ecsWorld->entity().set_name(entityName);

                // Add components
                if (isTranformCompontentPresent)
                {
                    newE.set<TransfromComponent>({
                        glm::vec3(0.f, 0.f, 0.f),
                        glm::vec3(0.f, 0.f, 0.f),
                        glm::vec3(50.f, 50.f, 50.f)
                    });
                }
                if (isMeshComponentPresent)
                {
                    newE.set<MeshComponent>({ meshName });
                }
                if (isAnimationComponentPresent)
                {
                    newE.set<AnimationComponent>({ true });
                }
                if (isLightingComponentPresent)
                {
                    GLTFLightType gltfLightType = GLTFLightType::NoLight;
                    if (strcmp(lightingType, "NoLighting") == 0)
                    {
                        gltfLightType = GLTFLightType::NoLight;
                    }
                    else if (strcmp(lightingType, "Directional") == 0)
                    {
                        gltfLightType = GLTFLightType::Directional;
                    }

                    newE.set<LightingComponent>({
                        gltfLightType,
                        glm::vec3(12.0f, 45.0f, 78.0f),
                        glm::vec3(0.8f, 0.1f, 0.3f),
                        0.15f,
                        0.75f,
                        1.25f
                    });
                }

                std::cout << "Created entity\n";
            }
            else
            {
                std::cout << "[Failed] Entity Exists\n";
            }

            ecsWorld->defer_end();               // <-- Apply safely
        }

        mu_end_window(ctx);
    }

    //if (mu_begin_window(ctx, "Entity Manager", mu_rect(800, 10, 350, 400)))
    //{
    //    static char entityName[64] = "NewEntity";
    //    static int entityType = 0; // 0 = Static Mesh, 1 = Animated Mesh, 2 = Light

    //    static char meshName[128] = "Avocado.gltf"; // default mesh
    //    static bool addAnimation = false;

    //    // For Light creation
    //    static glm::vec3 lightPos = glm::vec3(0.f, 5.f, 0.f);
    //    static glm::vec3 lightColor = glm::vec3(1.f, 1.f, 1.f);
    //    static float amb = 0.2f, diff = 1.f, spec = 1.f;

    //    // ------------------------------
    //    // Entity Type Selection
    //    // ------------------------------
    //    mu_label(ctx, "Entity Name:");
    //    mu_textbox(ctx, entityName, sizeof(entityName));

    //    const char* typeNames[] = { "Static Mesh", "Animated Mesh", "Light" };

    //    mu_label(ctx, "Entity Type:");
    //    mu_layout_row(ctx, 1, nullptr, 0);
    //    for (int i = 0; i < 3; i++)
    //    {
    //        if (mu_button(ctx, typeNames[i]))
    //            entityType = i;
    //    }

    //    // ------------------------------
    //    // Static & Animated Mesh Options
    //    // ------------------------------
    //    if (entityType == 0 || entityType == 1)
    //    {
    //        mu_label(ctx, "Mesh File:");
    //        mu_textbox(ctx, meshName, sizeof(meshName));

    //        if (entityType == 1)
    //        {
    //            mu_label(ctx, "Animated Mesh");
    //            mu_checkbox(ctx, "Add Animation Component", (int*)&addAnimation);
    //        }
    //    }

    //    // ------------------------------
    //    // Light Creation Options
    //    // ------------------------------
    //    if (entityType == 2)
    //    {
    //        mu_label(ctx, "Light Position:");
    //        mu_slider(ctx, &lightPos.x, -100.f, 100.f);
    //        mu_slider(ctx, &lightPos.y, -100.f, 100.f);
    //        mu_slider(ctx, &lightPos.z, -100.f, 100.f);

    //        mu_label(ctx, "Light Color:");
    //        mu_slider(ctx, &lightColor.r, 0.f, 1.f);
    //        mu_slider(ctx, &lightColor.g, 0.f, 1.f);
    //        mu_slider(ctx, &lightColor.b, 0.f, 1.f);

    //        mu_label(ctx, "Ambient:");  mu_slider(ctx, &amb, 0.f, 5.f);
    //        mu_label(ctx, "Diffuse:");  mu_slider(ctx, &diff, 0.f, 5.f);
    //        mu_label(ctx, "Specular:"); mu_slider(ctx, &spec, 0.f, 5.f);
    //    }

    //    mu_layout_row(ctx, 1, nullptr, 10);  // 10px vertical spacing

    //    // ------------------------------
    //    // Create Entity Button
    //    // ------------------------------
    //    if (mu_button(ctx, "Create Entity"))
    //    {
    //        flecs::entity e = ecsWorld->entity(entityName);

    //        if (entityType == 0) // Static Mesh
    //        {
    //            e.set<TransfromComponent>({
    //                glm::vec3(0.f, 0.f, 0.f),
    //                glm::vec3(0.f, 0.f, 0.f),
    //                glm::vec3(5.f, 5.f, 5.f)
    //                })
    //                .set<MeshComponent>({
    //                    std::string(meshName)
    //                    });
    //        }
    //        else if (entityType == 1) // Animated Mesh
    //        {
    //            e.set<TransfromComponent>({
    //                glm::vec3(0.f, 0.f, 0.f),
    //                glm::vec3(0.f, 0.f, 0.f),
    //                glm::vec3(5.f, 5.f, 5.f)
    //                })
    //                .set<MeshComponent>({ std::string(meshName) });

    //            if (addAnimation)
    //                e.set<AnimationComponent>({ true });
    //        }
    //        else if (entityType == 2) // Light
    //        {
    //            e.set<LightingComponent>({
    //                GLTFLightType::Directional,
    //                lightPos,
    //                lightColor,
    //                amb,
    //                diff,
    //                spec
    //                });
    //        }
    //    }

    //    mu_layout_row(ctx, 1, nullptr, 10);  // 10px vertical spacing

    //    // ------------------------------
    //    // Delete Entities
    //    // ------------------------------
    //    mu_label(ctx, "Delete Existing Entities:");

    //    ecsWorld->each([this](flecs::entity e)
    //        {
    //            char deleteName[128];
    //            sprintf(deleteName, "Delete %s", e.name());

    //            if (mu_button(ctx, deleteName))
    //            {
    //                e.destruct();
    //            }
    //        });

    //    mu_end_window(ctx);
    //}
}

void DebugMenuUISystem::EndRenderMenuUISystem()
{
    mu_end(ctx);
}
