#include "DebugMenuUISystem.h"

DebugMenuUISystem::DebugMenuUISystem(
    mu_Context* ctx, 
    std::shared_ptr<flecs::world> ecsWorld,
    std::shared_ptr<ReactPhysics3DCore> reactPhysics3DCore
)
	:
	ctx{ctx},
	ecsWorld{ecsWorld},
    reactPhysics3DCore{ reactPhysics3DCore }
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
    lightingQuery = ecsWorld->query_builder<TransfromComponent, LightingComponent>()
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

                    // -- Material Sliders--
                    mu_label(ctx, "Ambient:");  mu_slider(ctx, &mesh.ambientStrength, 0, 2);
                    mu_label(ctx, "Diffuse:");  mu_slider(ctx, &mesh.diffuseStrength, 0, 5);
                    mu_label(ctx, "Specular:"); mu_slider(ctx, &mesh.specularStrength, 0, 5);

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

                    // -- Material Sliders--
                    mu_label(ctx, "Ambient:");  mu_slider(ctx, &mesh.ambientStrength, 0, 5);
                    mu_label(ctx, "Diffuse:");  mu_slider(ctx, &mesh.diffuseStrength, 0, 5);
                    mu_label(ctx, "Specular:"); mu_slider(ctx, &mesh.specularStrength, 0, 5);

                    mu_end_treenode(ctx);
                }
                });
        }

        // Lighting Entities
        if (mu_header(ctx, "Lighting Entities"))
        {
            lightingQuery.each([this](flecs::entity e, TransfromComponent& transfrom ,LightingComponent& light)
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
                        GLTFLightType gltfLightType = light.lightType;
                        if (gltfLightType == GLTFLightType::NoLight)
                        {
                            sprintf(buf, "Type: %s", "NoLight");
                        }
                        else if (gltfLightType == GLTFLightType::Directional)
                        {
                            sprintf(buf, "Type: %s", "Directional");
                        }
                        mu_label(ctx, buf);
                        int widths[] = { 50, -1 };
                        mu_layout_row(ctx, 2, widths, 0);
                        // --- Position sliders ---
                        mu_label(ctx, "Pos X:"); mu_slider(ctx, &transfrom.Position.x, -(myMaxMin+fabsf(transfrom.Position.x)), myMaxMin+fabsf(transfrom.Position.x));
                        mu_label(ctx, "Pos Y:"); mu_slider(ctx, &transfrom.Position.y, -(myMaxMin+fabsf(transfrom.Position.y)), myMaxMin+fabsf(transfrom.Position.y));
                        mu_label(ctx, "Pos Z:"); mu_slider(ctx, &transfrom.Position.z, -(myMaxMin+fabsf(transfrom.Position.z)), myMaxMin+fabsf(transfrom.Position.z));

                        // --- Color sliders ---
                        mu_label(ctx, "Color R:"); mu_slider(ctx, &light.lightColor.r, 0, 1);
                        mu_label(ctx, "Color G:"); mu_slider(ctx, &light.lightColor.g, 0, 1);
                        mu_label(ctx, "Color B:"); mu_slider(ctx, &light.lightColor.b, 0, 1);

                        // --- Strength sliders ---
                        mu_label(ctx, "Ambient:");  mu_slider(ctx, &light.ambientStrength, 0.0f, 2.0f);
                        mu_label(ctx, "Diffuse:");  mu_slider(ctx, &light.diffuseStrength, 0.0f, 5.0f);
                        mu_label(ctx, "Specular:"); mu_slider(ctx, &light.specularStrength, 0.0f, 5.0f);

                        // --- Point Light Parameters sliders ---
                        mu_label(ctx, "Constant:");  mu_slider(ctx, &light.constant, 1.0f, 1.0f);     // FIXED
                        mu_label(ctx, "Linear:");    mu_slider(ctx, &light.linear, 0.0f, 1.0f);
                        mu_label(ctx, "Quadratic:"); mu_slider(ctx, &light.quadratic, 0.0f, 2.0f);

                        // -- Stop Light Parameters sliders --
                        static glm::vec3 tempLightDirection = glm::vec3{ 1.f };
                        mu_label(ctx, "Direction X:"); mu_slider(ctx, &tempLightDirection.x, -1.0f, 1.0f);
                        mu_label(ctx, "Direction Y:"); mu_slider(ctx, &tempLightDirection.y, -1.0f, 1.0f);
                        mu_label(ctx, "Direction Z:"); mu_slider(ctx, &tempLightDirection.z, -1.0f, 1.0f);
                        light.direction = tempLightDirection;

                        static float temp = 0;
                        mu_label(ctx, "CutOff:"); mu_slider(ctx, &temp, 0, 90);
                        light.cutOff = cos(glm::radians(temp));

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
        if (mu_header(ctx, "CREATE ENTITIES"))
        {
            static char entityName[64] = "NewEntity";
            static char meshName[64] = "NewMesh";
            static char lightingType[64] = "NoLighting";
            static char physicsType[64] = "BoxStatic";
            static int isTranformCompontentPresent = false;
            static int isMeshComponentPresent = false;
            static int isAnimationComponentPresent = false;
            static int isLightingComponentPresent = false;
            static int isPhysicsComponentPresent = false;

            int widths[] = { 100, -1 };
            mu_layout_row(ctx, 2, widths, 0);
            mu_label(ctx, "Entity Name:");
            mu_textbox(ctx, entityName, sizeof(entityName));

            mu_label(ctx, "Mesh Name:");
            mu_textbox(ctx, meshName, sizeof(meshName));

            mu_label(ctx, "Lighting Type: ");
            mu_textbox(ctx, lightingType, sizeof(lightingType));

            mu_label(ctx, "Physics Type: ");
            mu_textbox(ctx, physicsType, sizeof(physicsType));


            // --- Switch back to single-column layout ---
            mu_layout_row(ctx, 1, nullptr, 0);   // 1 column = full-width items

            // Now everything becomes vertical (top-to-bottom)
            mu_label(ctx, "Components:");
            mu_checkbox(ctx, "Transform Component", &isTranformCompontentPresent);
            mu_checkbox(ctx, "Mesh Component", &isMeshComponentPresent);
            mu_checkbox(ctx, "Animation Component", &isAnimationComponentPresent);
            mu_checkbox(ctx, "Lighting Component", &isLightingComponentPresent);
            mu_checkbox(ctx, "Physics Component", &isPhysicsComponentPresent);

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
                            glm::vec3(1.f, 1.f, 1.f)
                            });
                    }
                    if (isMeshComponentPresent)
                    {
                        newE.set<MeshComponent>({
                            meshName,
                            1, 1, 1
                            });
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
                        else if (strcmp(lightingType, "PointLight") == 0)
                        {
                            gltfLightType = GLTFLightType::PointLight;
                        }
                        else if (strcmp(lightingType, "SpotLight") == 0)
                        {
                            gltfLightType = GLTFLightType::SpotLight;
                        }

                        newE.set<LightingComponent>({
                            gltfLightType,
                            glm::vec3(12.0f, 45.0f, 78.0f),
                            glm::vec3(0.8f, 0.1f, 0.3f),
                            0.15f,
                            0.75f,
                            1.25f,
                            1,
                            0.7,
                            1.8,
                            glm::vec3{1.f},
                            10.f
                            });
                    }
                    if (isPhysicsComponentPresent)
                    {
                        PHYSICSCOLLOIDERTYPE colloiderType = PHYSICSCOLLOIDERTYPE::BOXCOLLOIDER;
                        PHYSICSBODYTYPE bodyType = PHYSICSBODYTYPE::STATICMESH;

                        if (strcmp(physicsType, "BoxStatic") == 0) {
                            colloiderType = PHYSICSCOLLOIDERTYPE::BOXCOLLOIDER;
                            bodyType = PHYSICSBODYTYPE::STATICMESH;
                        }
                        else if (strcmp(physicsType, "BoxDynamic") == 0) {
                            colloiderType = PHYSICSCOLLOIDERTYPE::BOXCOLLOIDER;
                            bodyType = PHYSICSBODYTYPE::DYMAMICMESH;
                        }
                        else if (strcmp(physicsType, "BoxKinematic") == 0) {
                            colloiderType = PHYSICSCOLLOIDERTYPE::BOXCOLLOIDER;
                            bodyType = PHYSICSBODYTYPE::KINEMATICMESH;
                        }
                        else if (strcmp(physicsType, "SphereStatic") == 0) {
                            colloiderType = PHYSICSCOLLOIDERTYPE::SPHERECOLLOIDER;
                            bodyType = PHYSICSBODYTYPE::STATICMESH;
                        }
                        else if (strcmp(physicsType, "SphereDynamic") == 0) {
                            colloiderType = PHYSICSCOLLOIDERTYPE::SPHERECOLLOIDER;
                            bodyType = PHYSICSBODYTYPE::DYMAMICMESH;
                        }
                        else if (strcmp(physicsType, "SphereKinematic") == 0) {
                            colloiderType = PHYSICSCOLLOIDERTYPE::SPHERECOLLOIDER;
                            bodyType = PHYSICSBODYTYPE::KINEMATICMESH;
                        }

                        newE.set<PhysicsComponent>({
                            colloiderType,
                            bodyType,
                            true
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
        }

        if (mu_header(ctx, "DESTROY ENTITIES"))
        {
            static char entityName[64] = "NewEntity";

            int widths[] = { 100, -1 };
            mu_layout_row(ctx, 2, widths, 0);
            mu_label(ctx, "Entity Name:");
            mu_textbox(ctx, entityName, sizeof(entityName));

            if (mu_button(ctx, "DESTROY ENTITY"))
            {
                ecsWorld->defer_begin();

                flecs::entity e = ecsWorld->lookup(entityName);
                if (e.is_valid()) {
                    std::string deletedEntityName = e.name().c_str();
                    e.destruct();
                    std::cout << "Deleted Entity " << deletedEntityName << "\n";
                }
                else {
                    std::cout << "[Failed] Entity not found\n";
                }

                ecsWorld->defer_end();
            }

        }

        mu_end_window(ctx);
    }
}

void DebugMenuUISystem::GlobalMenu()
{
    if (mu_begin_window(ctx, "Global Menu", mu_rect(320, 240, 200, 260)))
    {
        // ---- Sun ----
        mu_label(ctx, "Sun Position : ");
        int widths[] = { 40, -1 };
        mu_layout_row(ctx, 2, widths, 0);

        mu_label(ctx, "X : "); mu_slider(ctx, &GlobalInformation::sunLightDirection.x, -1, 1);
        mu_label(ctx, "Y : "); mu_slider(ctx, &GlobalInformation::sunLightDirection.y, -1, 1);
        mu_label(ctx, "Z : "); mu_slider(ctx, &GlobalInformation::sunLightDirection.z, -1, 1);
        mu_label(ctx, "Distance : "); mu_slider(ctx, &GlobalInformation::sunLightDistanceFactor, 0, 100);

        // ---- Toggles ----
        int width[] = { 140, -1 };
        mu_layout_row(ctx, 2, width, 0);

        static int renderShadow = GlobalInformation::renderShadowPass;
        mu_label(ctx, "Render Shadow Pass:");
        mu_checkbox(ctx, "", &renderShadow);
        GlobalInformation::renderShadowPass = (renderShadow != 0);

        static int physicsDebug = GlobalInformation::IsReactPhysics3DDebuggerEnabled;
        mu_label(ctx, "ReactPhysics3D Debug:");
        mu_checkbox(ctx, "", &physicsDebug);
        GlobalInformation::IsReactPhysics3DDebuggerEnabled = (physicsDebug != 0);

        // ---- Commit ----
        GlobalInformation::IsReactPhysics3DStateCommited = false;
        mu_label(ctx, "ReactPhysics3D Commit:");
        if (mu_button(ctx, "COMMIT")) {
            GlobalInformation::IsReactPhysics3DStateCommited = true;
        }

        // ---- Scene ----
        static int scenePlaying = 0;
        mu_label(ctx, "START SCENE:");
        mu_checkbox(ctx, "", &scenePlaying);
        GlobalInformation::IsScenePlaying = (scenePlaying != 0);

        mu_end_window(ctx);
    }
}

void DebugMenuUISystem::EndRenderMenuUISystem()
{
    mu_end(ctx);
}
