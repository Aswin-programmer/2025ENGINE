// Force NVIDIA GPU
extern "C" {

__declspec(dllexport) unsigned long NvOptimusEnablement = 0x00000001;
}

#include <iostream>
#include <memory>

#include <GlobalInformation/GlobalInformation.h>

#include <WINDOW/Window.h>

#include <MEMORY/Memory.h>

#include <INPUT/KeyBoard.h>
#include <INPUT/Mouse.h>

#include <CAMERA/EditorCamera.h>

#include <RENDERER/MESH/GLTFMESH/GLTFMESHLoader.h>
#include <RENDERER/MESH/GLTFMESH/GLTFMESHRenderer.h>
#include <RENDERER/SHADERS/Shader.h>
#include <RENDERER/TEXTURE_KTX/TextureKTX2.h>

#include <RENDERER/FRAME_BUFFER/DepthMap.h>

#include <ECS/COMPONENTS/AnimationComponent.h>
#include <ECS/COMPONENTS/IdentiferComponent.h>
#include <ECS/COMPONENTS/LightingComponent.h>
#include <ECS/COMPONENTS/MeshComponent.h>
#include <ECS/COMPONENTS/PhysicsComponent.h>
#include <ECS/COMPONENTS/TransfromComponent.h>
#include <ECS/ECSWorld.h>

#include <ECS/SYSTEMS/DebugMenuUISystem.h>
#include <ECS/SYSTEMS/MeshRendererSystem.h>

#include <DEBUGUI/MicroUIRenderer.h>

#include <SCRIPTING/NativeCPP/NativeCPPGlobalScript.h>
#include <SCRIPTING/NativeCPP/NativeCPPScriptManager.h>

#include <REACTPHYSICS3D/ReactPhysics3DCore.h>
#include <REACTPHYSICS3D/ReactPhysics3DDebugRenderer.h>
#include <REACTPHYSICS3D/ReactPhysics3DSystem.h>

#include <SERIALIZATION/Serialization.h>

#include <RENDERER/INGAMEUI/InGameUIRenderer.h>

// Opengl Callbacks  
void processKeyInput(GLFWwindow *window);
void mouse_pos_callback(GLFWwindow *window, double xposIn, double yposIn);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);
void mouse_button_callback(GLFWwindow *window, int button, int action, int mods);
void char_callback(GLFWwindow *window, unsigned int codepoint);

// Editor Camera Setup
EditorCamera camera(45.0f, 640.f / 480.f, 0.1f, 2000.0f);

// Shadow Map Camera Setup
EditorCamera shadowCamera(45.0f, 640.f / 480.f, 0.1f, 2000.0f);

// Mouse Setup
float GlobalMousePosX = 0.f;
float GlobalMousePosY = 0.f;

// Debug Global Setting
float GlobalDebugWindowShow = false;

void OliUI_GLFWInputProcessing(OliUIMainContext* oliUIMainContext, GLFWwindow* window);

int main() {
    Window::init("TESTING");

    std::shared_ptr<Memory> memory = std::make_shared<Memory>();

    Keyboard::Init();
    Mouse::Init();

    // Initialzing DebugUI
    MicroUIRenderer::InitMicroUIRenderer();

    glfwSetCursorPosCallback(Window::getGLFWWindow(), mouse_pos_callback);
    glfwSetScrollCallback(Window::getGLFWWindow(), scroll_callback);
    glfwSetKeyCallback(Window::getGLFWWindow(), key_callback);
    glfwSetMouseButtonCallback(Window::getGLFWWindow(), mouse_button_callback);
    glfwSetCharCallback(Window::getGLFWWindow(), char_callback);

    Shader shader = Shader((std::string(RESOURCES_PATH) + "SHADER/cube.vert").c_str(),
                           (std::string(RESOURCES_PATH) + "SHADER/cube.frag").c_str());

    TextureKTX2 textureKTX2 = TextureKTX2(
        (std::string(RESOURCES_PATH) + "TEXTURE/KTX/cube.ktx2").c_str(), TextureType::NORMAL{});

    Shader shader2 =
        Shader((std::string(RESOURCES_PATH) + "SHADER/GEO_TEST/geo_vert.glsl").c_str(),
               (std::string(RESOURCES_PATH) + "SHADER/GEO_TEST/geo_frag.glsl").c_str(), nullptr,
               nullptr, (std::string(RESOURCES_PATH) + "SHADER/GEO_TEST/geo_geo.glsl").c_str());

    Shader shader3 = Shader(
        (std::string(RESOURCES_PATH) + "SHADER/PARTICLE_SYSTEMS/particle_vert.glsl").c_str(),
        (std::string(RESOURCES_PATH) + "SHADER/PARTICLE_SYSTEMS/particle_frag.glsl").c_str(),
        nullptr, nullptr,
        (std::string(RESOURCES_PATH) + "SHADER/PARTICLE_SYSTEMS/geometry_particle_shader.glsl")
            .c_str());

    /*if (!GLTFMESHLoader::LoadGLTFModel(std::string(RESOURCES_PATH) +
    "GLTFMODEL/MIXED_MODEL/mix.gltf"))
    {
        std::cout << "Failed to load the sample model!\n";
    }
    if (!GLTFMESHLoader::LoadGLTFModel(std::string(RESOURCES_PATH) + "GLTFMODEL/CUBE/cube.gltf"))
    {
        std::cout << "Failed to load the sample model!\n";
    }
    if (!GLTFMESHLoader::LoadGLTFModel(std::string(RESOURCES_PATH) +
    "GLTFMODEL/AVOCADO/Avocado.gltf"))
    {
        std::cout << "Failed to load the sample model!\n";
    }
    if (!GLTFMESHLoader::LoadGLTFModel(std::string(RESOURCES_PATH) +
    "GLTFMODEL/FISH/BarramundiFish.gltf"))
    {
        std::cout << "Failed to load the sample model!\n";
    }*/
    if (!GLTFMESHLoader::LoadGLTFModel(std::string(RESOURCES_PATH) +
                                           "GLTFMODEL/ANIMATED_TESTING/AnimatedTesting.gltf",
                                       true, false)) {
        std::cout << "Failed to load the sample model!\n";
    }

    if (!GLTFMESHLoader::LoadGLTFModel(std::string(RESOURCES_PATH) +
                                           "GLTFMODEL/ANIMATED_TESTING_2/AnimatedTesting2.gltf",
                                       true, false)) {
        std::cout << "Failed to load the sample model!\n";
    }

    if (!GLTFMESHLoader::LoadGLTFModel(std::string(RESOURCES_PATH) +
                                           "GLTFMODEL/ANIMATED_TESTING_3/AnimatedTesting3.gltf",
                                       true, false)) {
        std::cout << "Failed to load the sample model!\n";
    }

    if (!GLTFMESHLoader::LoadGLTFModel(
            std::string(RESOURCES_PATH) + "GLTFMODEL/AVOCADO/Avocado.gltf", false, false)) {
        std::cout << "Failed to load the sample model!\n";
    }

    if (!GLTFMESHLoader::LoadGLTFModel(std::string(RESOURCES_PATH) + "GLTFMODEL/LIGHT/light.gltf",
                                       false, false)) {
        std::cout << "Failed to load the sample model!\n";
    }

    if (!GLTFMESHLoader::LoadGLTFModel(std::string(RESOURCES_PATH) + "GLTFMODEL/CUBE/cube.gltf",
                                       false, false)) {
        std::cout << "Failed to load the sample model!\n";
    }
    if (!GLTFMESHLoader::LoadGLTFModel(std::string(RESOURCES_PATH) + "GLTFMODEL/PHYSICS1/box.gltf",
                                       false, true)) {
        std::cout << "Failed to load the sample model!\n";
    }
    if (!GLTFMESHLoader::LoadGLTFModel(std::string(RESOURCES_PATH) + "GLTFMODEL/PHYSICS2/box2.gltf",
                                       false, true)) {
        std::cout << "Failed to load the sample model!\n";
    }
    if (!GLTFMESHLoader::LoadGLTFModel(std::string(RESOURCES_PATH) +
                                           "GLTFMODEL/PHYSICS3/AnimatedColloider.gltf",
                                       true, true)) {
        std::cout << "Failed to load the sample model!\n";
    }

    // ######### Setting Up The ECS      ##########

    std::shared_ptr<ECSWorld> ecsWorld = std::make_shared<ECSWorld>();
    ecsWorld->InitECSWorld();

    // ############################################

    // ######### Setting Up The Renderer ##########

    std::shared_ptr<MeshRendererSystem> meshRenderSystem = std::make_shared<MeshRendererSystem>(
        (std::string(RESOURCES_PATH) + "SHADER/GLTF_MODEL/gltf_vert.glsl"),
        (std::string(RESOURCES_PATH) + "SHADER/GLTF_MODEL/gltf_frag.glsl"), ecsWorld->GetWorld());
    meshRenderSystem->InitMeshRendererSystem();

    // ############################################

    // #################################################
    std::shared_ptr<Shader> shaderDepthMap = std::make_shared<Shader>(
        (std::string(RESOURCES_PATH) + "SHADER/SHADOW_MAP/depth_vert.glsl").c_str(),
        (std::string(RESOURCES_PATH) + "SHADER/SHADOW_MAP/depth_frag.glsl").c_str(), nullptr,
        nullptr, nullptr);

    Shader shaderDepthDebugMap =
        Shader((std::string(RESOURCES_PATH) + "SHADER/SHADOW_MAP/depth_debug_vert.glsl").c_str(),
               (std::string(RESOURCES_PATH) + "SHADER/SHADOW_MAP/depth_debug_frag.glsl").c_str(),
               nullptr, nullptr, nullptr);

    std::shared_ptr<DepthMap> depthMap = std::make_shared<DepthMap>(640, 480);
    depthMap->DepthMapInit();
    // depthMap->BindDepthTextureToTextureUnit(20);

    // #################################################

    // ##################################################

    GlobalInformation::InitSun();

    // #################################################

    // ##          ##

    /*flecs::entity e1 = ecsWorld->CreateEntity("Test1");
    e1
        .set<TransfromComponent>({ glm::vec3(0.f, 0.f, 0.f)
        , glm::vec3(0.f, 0.f, 0.f), glm::vec3(50.f, 50.f, 50.f) })
        .set<MeshComponent>({
            "Avocado.gltf",
            1, 1, 1
        });

    flecs::entity e2 = ecsWorld->CreateEntity("Test2");
    e2
        .set<TransfromComponent>({ glm::vec3(0.f, 2.f, 0.f)
        , glm::vec3(0.f, 0.f, 0.f), glm::vec3(50.f, 50.f, 50.f) })
        .set<MeshComponent>({
            "Avocado.gltf",
            1, 1, 1
        });

    flecs::entity e3 = ecsWorld->CreateEntity("Test3");
    e3
        .set<TransfromComponent>({ glm::vec3(0.f, 0.f, 0.f)
        , glm::vec3(0.f, 0.f, 0.f), glm::vec3(5.f, 5.f, 5.f) })
        .set<MeshComponent>({
            "AnimatedTesting.gltf",
            1, 1, 1
        })
        .set<AnimationComponent>({ true });

    flecs::entity e4 = ecsWorld->CreateEntity("Test4");
    e4
        .set<TransfromComponent>({ glm::vec3(50.f, 0.f, 0.f)
        , glm::vec3(0.f, 0.f, 0.f), glm::vec3(3.f, 3.f, 3.f) })
        .set<MeshComponent>({
            "AnimatedTesting2.gltf",
            1, 1, 1
        })
        .set<AnimationComponent>({ true });

    flecs::entity e5 = ecsWorld->CreateEntity("Test5");
    e5
        .set<TransfromComponent>({ glm::vec3(10.f, 0.f, 0.f)
        , glm::vec3(0.f, 0.f, 0.f), glm::vec3(1.f, 1.f, 1.f) })
        .set<MeshComponent>({
            "AnimatedTesting3.gltf",
            1, 1, 1
        })
        .set<AnimationComponent>({ true });

    flecs::entity e6 = ecsWorld->CreateEntity("Test6");
    e6
        .set<TransfromComponent>({ glm::vec3(15.f, 0.f, 0.f)
        , glm::vec3(0.f, 0.f, 0.f), glm::vec3(1.f, 1.f, 1.f) })
        .set<MeshComponent>({
            "AnimatedTesting3.gltf",
            1, 1, 1
        })
        .set<AnimationComponent>({ true });*/

    flecs::entity e7 = ecsWorld->CreateEntity("Test7");
    e7.set<IdentiferComponent>({"Test7"})
        .set<TransfromComponent>(
            {glm::vec3(15.f, 0.f, 0.f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(1.f, 1.f, 1.f)})
        .set<LightingComponent>({GLTFLightType::Directional, glm::vec3(12.0f, 45.0f, 78.0f),
                                 glm::vec3(0.8f, 0.1f, 0.3f), 0.15f, 0.75f, 1.25f, 1.0f, 0.7f, 1.8f,
                                 glm::vec3{1.f}, 1.f});

    /*flecs::entity e9 = ecsWorld->CreateEntity("Test9");
    e9
        .set<TransfromComponent>({ glm::vec3(0.f), glm::vec3(0.f), glm::vec3(1.f) })
        .set<MeshComponent>({ "box.gltf", 1, 1, 1 })
        .set<PhysicsComponent>({ PHYSICSCOLLOIDERTYPE::BOXCOLLOIDER, PHYSICSBODYTYPE::STATICMESH,
    true });*/

    // ## Testing some Scripting Stuff ##

    // ## Testing some Scripting Stuff ##

    /*std::shared_ptr<NativeCPPScriptManager> nativeCPPScriptManager =
        std::make_shared<NativeCPPScriptManager>(SCRIPT_PATH + std::string("ScriptImpl.dll"));
    nativeCPPScriptManager->LoadDependencies(ecsWorld);
    nativeCPPScriptManager->LoadDLL();*/

    // auto world = ecsWorld->GetWorld();

    // world->defer_begin();  // start deferring table modifications

    // world->query<TransfromComponent>().each([](flecs::entity e, TransfromComponent& p) {
    //	e.destruct();  // this is deferred safely
    // });

    // world->defer_end(); // apply all deferred operations
    //// Again, if this is not inside a system, you might need a world progress call.
    //// flecsWorld->progress();

    // Setting up the ReactPhysics3D
    std::shared_ptr<ReactPhysics3DCore> physicsCore = std::make_shared<ReactPhysics3DCore>();
    physicsCore->InitReactPhysics3DCore();
    physicsCore->SetDebug(GlobalInformation::IsReactPhysics3DDebuggerEnabled);

    // Setting up the ReactPhysics3DSystem
    std::shared_ptr<ReactPhysics3DSystem> physicsSystem =
        std::make_shared<ReactPhysics3DSystem>(physicsCore, ecsWorld->GetWorld());

    rp3d::PhysicsWorld *world = physicsCore->GetPhysicsWorld();
    std::shared_ptr<rp3d::PhysicsCommon> physicsCommon = physicsCore->GetPhysicsCommon();

    // ############################################

    std::shared_ptr<DebugMenuUISystem> debugMenuUISystem = std::make_shared<DebugMenuUISystem>(
        MicroUIRenderer::GetContext(), ecsWorld->GetWorld(), physicsCore);
    debugMenuUISystem->InitDebugMenuUISystem();

    // #################################################

    std::shared_ptr<ReactPhysicsDebugRenderer> debugRenderer =
        std::make_shared<ReactPhysicsDebugRenderer>(
            (std::string(RESOURCES_PATH) + "SHADER/REACTDEBUG/debug_vert.glsl").c_str(),
            (std::string(RESOURCES_PATH) + "SHADER/REACTDEBUG/debug_frag.glsl").c_str(),
            physicsCore);
     
    // Serialization Stuff
    Serialization::SetSerializationPath(std::string(RESOURCES_PATH) + "SERIALIZATION/");

    // ########### INGAME UI STUFF ###################
     
    TextureLoader::AddKTX2Texture(
        std::string(RESOURCES_PATH) + "TEXTURE/INGAMEUI/inGameUI.ktx2", 
        "inGameUI.ktx2", 
        TextureType::NORMAL{}
    );

    std::unordered_map<std::string, InGameUISpriteRect> sprites;
    sprites["START"] = InGameUISpriteRect(0, 96, 64, 32);
    sprites["CONTINUE"] = InGameUISpriteRect(0, 64, 64, 32);

    std::shared_ptr<InGameUIRenderer> inGameUIRenderer = std::make_shared<InGameUIRenderer>(
        std::string("inGameUI.ktx2"),
        InGameUISprite(std::string("inGameUI.ktx2"), 128, 128, sprites),
        std::string(RESOURCES_PATH) + "SHADER/INGAMEUI/InGameUI.vert",
        std::string(RESOURCES_PATH) + "SHADER/INGAMEUI/InGameUI.frag"
    ); 

    inGameUIRenderer->InitializeInGameUIRenderer();

    OliUIMainContext* oliUIMainContext = (OliUIMainContext*)malloc(sizeof(OliUIMainContext));
    OliUIInit(oliUIMainContext);

    // ###############################################

    while (!Window::shouldClose()) {
        if (Keyboard::IsKeyPressed(KEY_F12)) {
            memory->PrintCurrentMemoryConsumption();
        }

        ecsWorld->GetWorld()->progress();
        Window::clearScreen();
        Window::processInput();

        processKeyInput(Window::getGLFWWindow());

        // Create projection matrices [PERSPECTIVE]
        glm::mat4 projectionP = glm::mat4(1.0f);
        projectionP = glm::perspective(glm::radians(45.0f), (float)640 / (float)480, 0.1f, 2000.0f);

        // Create A Orthogonal Projection Matrix [ORTHOGONAL]
        glm::mat4 projectionO = glm::mat4(1.f);
        projectionO = glm::ortho(-32.f, 32.f, -24.f, 24.f, 0.1f, 200.f);

        // Camera or View transformation
        glm::mat4 view = camera.GetViewMatrix();
        shader3.setMat4("view", view);

        // glm::vec3 lightTarget = glm::vec3(0.0f, 0.0f, 0.0f); // Look at scene center
        // glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
        // glm::mat4 lightViewMatrix = glm::lookAt(shadowCamera.GetPosition(), lightTarget,
        // worldUp);

        flecs::entity e = ecsWorld->GetWorld()->entity("Test7");
        auto &transfrom = e.get_mut<TransfromComponent>();

        transfrom.Position = shadowCamera.GetPosition();

        meshRenderSystem->MeshRendererUpdate();

        depthMap->DepthMapBind();
        meshRenderSystem->MeshRendererFinalDrawCall(shadowCamera.GetViewMatrix(), projectionO,
                                                    camera.GetPosition(), shaderDepthMap,
                                                    shadowCamera.GetViewMatrix(), projectionO);
        depthMap->DepthMapUnBind();

        // ######## Handling InGameUI stuff #######

        OliUI_GLFWInputProcessing(oliUIMainContext, Window::getGLFWWindow());
          
        OliUIBegin(oliUIMainContext);   

        OliUIBeginWindowContainer(oliUIMainContext, OliUIRect{160, 120, 320, 240});
        OliUIBeginLayout(oliUIMainContext, std::array<int, 5>{40, 60, 40, 60, 40}.data(), 5, OliUILayoutVertical);
        
        OliUIBeginSubWindowContainer(oliUIMainContext, "temp1", sizeof("temp1"));
        OliUIEndSubWindowContainer(oliUIMainContext);

        OliUIMouseClickState state1 = OliUIBeginSubWindowContainer(oliUIMainContext, "first", sizeof("first"));
        if(state1 & OliUIJustClicked){
            std::cout<<"The Button Is Clicked-1."<<std::endl;
        }
        OliUIDrawTexturedRectangle(oliUIMainContext, OliUITexture{"START"});
        OliUIEndSubWindowContainer(oliUIMainContext);

        OliUIBeginSubWindowContainer(oliUIMainContext, "temp2", sizeof("temp2"));
        OliUIEndSubWindowContainer(oliUIMainContext);

        OliUIMouseClickState state2 = OliUIBeginSubWindowContainer(oliUIMainContext, "second", sizeof("second"));
        if (state2 & OliUIJustClicked) {
            std::cout << "The Button Is Clicked-2." << std::endl;
        }
        OliUIDrawTexturedRectangle(oliUIMainContext, OliUITexture{ "CONTINUE" });
        OliUIEndSubWindowContainer(oliUIMainContext);

        OliUIBeginSubWindowContainer(oliUIMainContext, "temp3", sizeof("temp3"));
        OliUIEndSubWindowContainer(oliUIMainContext);

        OliUIEndLayout(oliUIMainContext);
        OliUIEndWindowContainer(oliUIMainContext); 


        OliUIEnd(oliUIMainContext);

        inGameUIRenderer->CleanUpInGameUIRenderer();
        inGameUIRenderer->OliUIInspectDrawCommands(oliUIMainContext);
        inGameUIRenderer->RenderInGameUI();

        // ########################################

        if (GlobalInformation::renderShadowPass) {
            depthMap->RenderDebugDepthMap(shaderDepthDebugMap);
        } else {
            depthMap->BindDepthTextureToTextureUnit(20);
            meshRenderSystem->MeshRendererFinalDrawCall(view, projectionP, camera.GetPosition(),
                                                        nullptr, shadowCamera.GetViewMatrix(),
                                                        projectionO);
        }

        // Handling the physics debug rendering:
        if (GlobalInformation::IsScenePlaying) {
            world->update(1.f / 60.f);
        }
        debugRenderer->ReactPhysicsRendererRender(view, projectionP);
        physicsSystem->UpdateReactPhysics3DSystem();

        //nativeCPPScriptManager->UpdateScript();

        // std::cout<<"The FPS is : "<<Window::GetFPSValue()<<std::endl;

        if (Keyboard::IsKeyJustPressed(KEY_F5)) {
            GlobalDebugWindowShow = !GlobalDebugWindowShow;
        }

        if (GlobalDebugWindowShow) {

            debugMenuUISystem->StartRenderMenuUISystem();
            debugMenuUISystem->RenderUIMenu();
            debugMenuUISystem->PerformanceUIMenu();
            debugMenuUISystem->EntityManagerMenu();
            debugMenuUISystem->GlobalMenu();
            debugMenuUISystem->EndRenderMenuUISystem();

            // Snapshot 3
            MicroUIRenderer::render_debug_ui();
        }

        Mouse::Update();
        Keyboard::Update();
        Window::update();
    }  

    free(oliUIMainContext);
    MicroUIRenderer::DestroyMicroUIRenderer();
    Window::cleanup();
}

void processKeyInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
    /*if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(FORWARD, Window::getdt());
    }

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(BACKWARD, Window::getdt());
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(LEFT, Window::getdt());
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(RIGHT, Window::getdt());
    }*/
}

void mouse_pos_callback(GLFWwindow *window, double xposIn, double yposIn) {
    static bool firstMouse = true;
    static float lastX = 0.0f;
    static float lastY = 0.0f;

    if (firstMouse) {
        lastX = xposIn;
        lastY = yposIn;
        firstMouse = false;
    }

    float deltaX = xposIn - lastX;
    float deltaY = lastY - yposIn;

    lastX = xposIn;
    lastY = yposIn;

    // Right click = Orbit
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
        if (GlobalInformation::renderShadowPass) {
            shadowCamera.OnMouseRotate(deltaX, deltaY);
        } 
        else if(GlobalInformation::IsInGameUIRendered)
        {

        }
        else 
        {
            camera.OnMouseRotate(deltaX, deltaY);
        }
    }
    // Middle click = Pan
    else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS) {
        if (GlobalInformation::renderShadowPass) 
        {
            shadowCamera.OnMousePan(deltaX, deltaY);
        } 
        else if (GlobalInformation::IsInGameUIRendered)
        {

        }
        else 
        {
            camera.OnMousePan(deltaX, deltaY);
        }
    }

    Mouse::SetMouseMoving(true);

    if (GlobalDebugWindowShow) {
        mu_Context *ctx = MicroUIRenderer::GetContext();
        mu_input_mousemove(ctx, (int)xposIn, (int)yposIn);
    }
}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {
    if (GlobalInformation::renderShadowPass) {
        shadowCamera.OnMouseScroll((float)yoffset);
    } 
    else if (GlobalInformation::IsInGameUIRendered)
    {

    }
    else {
        camera.OnMouseScroll((float)yoffset);
    }

    Mouse::SetMouseWheelX(static_cast<int>(xoffset));
    Mouse::SetMouseWheelY(static_cast<int>(yoffset));

    if (GlobalDebugWindowShow) {
        mu_Context *ctx = MicroUIRenderer::GetContext();
        mu_input_scroll(ctx, 0, (int)(yoffset * -30));
    }
}

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    mu_Context *ctx = MicroUIRenderer::GetContext();
    int c = MicroUIRenderer::get_key_code(key);
    if (action == GLFW_PRESS) {
        Keyboard::OnKeyPressed(key);

        if (GlobalDebugWindowShow) {
            mu_input_keydown(ctx, c);
        }
    } else if (action == GLFW_RELEASE) {
        Keyboard::OnKeyReleased(key);

        if (GlobalDebugWindowShow) {
            mu_input_keyup(ctx, c);
        }
    }
}

void mouse_button_callback(GLFWwindow *window, int button, int action, int mods) {
    mu_Context *ctx = MicroUIRenderer::GetContext();
    int b = MicroUIRenderer::get_mouse_code(button);
    double x, y;
    glfwGetCursorPos(window, &x, &y);
    if (action == GLFW_PRESS) {
        Mouse::OnBtnPressed(button);

        if (GlobalDebugWindowShow) {
            mu_input_mousedown(ctx, (int)x, (int)y, b);
        }
    } else if (action == GLFW_RELEASE) {
        Mouse::OnBtnReleased(button);

        if (GlobalDebugWindowShow) {
            mu_input_mouseup(ctx, (int)x, (int)y, b);
        }
    }
}

void char_callback(GLFWwindow *window, unsigned int codepoint) {
    mu_Context *ctx = MicroUIRenderer::GetContext();
    char buf[5] = {0};
    if (codepoint <= 0x7F) {
        buf[0] = (char)codepoint;
    } else if (codepoint <= 0x7FF) {
        buf[0] = (char)(0xC0 | (codepoint >> 6));
        buf[1] = (char)(0x80 | (codepoint & 0x3F));
    } else if (codepoint <= 0xFFFF) {
        buf[0] = (char)(0xE0 | (codepoint >> 12));
        buf[1] = (char)(0x80 | ((codepoint >> 6) & 0x3F));
        buf[2] = (char)(0x80 | (codepoint & 0x3F));
    } else if (codepoint <= 0x10FFFF) {
        buf[0] = (char)(0xF0 | (codepoint >> 18));
        buf[1] = (char)(0x80 | ((codepoint >> 12) & 0x3F));
        buf[2] = (char)(0x80 | ((codepoint >> 6) & 0x3F));
        buf[3] = (char)(0x80 | (codepoint & 0x3F));
    }

    if (GlobalDebugWindowShow) {
        mu_input_text(ctx, buf);
    }
}

void OliUI_GLFWInputProcessing(OliUIMainContext* oliUIMainContext, GLFWwindow* window)
{
    OliUIInput* in = &oliUIMainContext->input;

    memcpy(in->mouseButtonsPrev, in->mouseButtons, sizeof(in->mouseButtons));

    int currentWidth = Window::getWidth();
    int currentHeight = Window::getHeight();

    float scaleX = 640.0f / currentWidth;
    float scaleY = 480.0f / currentHeight;

    auto mousePos = Mouse::GetMouseScreenPosition(window);

    in->mouseX = mousePos.first * scaleX;
    in->mouseY = mousePos.second * scaleY;

    in->mouseButtons[0] = Mouse::IsBtnJustPressed(MOUSE_LEFT);
    in->mouseButtons[1] = Mouse::IsBtnJustPressed(MOUSE_RIGHT);

    in->mouseScrollDelta = (float)Mouse::GetMouseWheelY();
}
