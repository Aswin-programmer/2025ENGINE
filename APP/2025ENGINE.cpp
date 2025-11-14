// Force NVIDIA GPU
extern "C" {

	__declspec(dllexport) unsigned long NvOptimusEnablement = 0x00000001;
}

#include <iostream>
#include <memory>

#include <GlobalInformation/GlobalInformation.h>
#include <WINDOW/Window.h>
#include <CAMERA/EditorCamera.h>
#include <RENDERER/TEXTURE_KTX/TextureKTX2.h>
#include <RENDERER/SHADERS/Shader.h>
#include <RENDERER/MESH/GLTFMESH/GLTFMESHLoader.h>
#include <RENDERER/MESH/GLTFMESH/GLTFMESHRenderer.h>

#include <ECS/ECSWorld.h>
#include <ECS/COMPONENTS/MeshComponent.h>
#include <ECS/COMPONENTS/TransfromComponent.h>
#include <ECS/SYSTEMS/MeshRendererSystem.h>

#include <SCRIPTING/NativeCPP/NativeCPPGlobalScript.h>
#include <SCRIPTING/NativeCPP/NativeCPPScriptManager.h>

// Opengl Callbacks
void processKeyInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

// Editor Camera Setup
EditorCamera camera(45.0f, 640.f / 480.f, 0.1f, 200.0f);

// Mouse Setup
float GlobalMousePosX = 0.f;
float GlobalMousePosY = 0.f;

int main() 
{
	std::shared_ptr<GlobalInformation> globalInformation = std::make_shared<GlobalInformation>();
	
	Window::init("TESTING");

	glfwSetCursorPosCallback(Window::getGLFWWindow(), mouse_callback);
	glfwSetScrollCallback(Window::getGLFWWindow(), scroll_callback);

	Shader shader = Shader((std::string(RESOURCES_PATH) + "SHADER/cube.vert").c_str()
		, (std::string(RESOURCES_PATH) + "SHADER/cube.frag").c_str());

	TextureKTX2 textureKTX2 = TextureKTX2((std::string(RESOURCES_PATH) + "TEXTURE/KTX/cube.ktx2").c_str(), TextureType::NORMAL{});

	Shader shader2 = Shader(
		(std::string(RESOURCES_PATH) + "SHADER/GEO_TEST/geo_vert.glsl").c_str(),
		(std::string(RESOURCES_PATH) + "SHADER/GEO_TEST/geo_frag.glsl").c_str(),
		nullptr,
		nullptr,
		(std::string(RESOURCES_PATH) + "SHADER/GEO_TEST/geo_geo.glsl").c_str()
	);

	Shader shader3 = Shader(
		(std::string(RESOURCES_PATH) + "SHADER/PARTICLE_SYSTEMS/particle_vert.glsl").c_str(),
		(std::string(RESOURCES_PATH) + "SHADER/PARTICLE_SYSTEMS/particle_frag.glsl").c_str(),
		nullptr,
		nullptr,
		(std::string(RESOURCES_PATH) + "SHADER/PARTICLE_SYSTEMS/geometry_particle_shader.glsl").c_str()
	);

	if (!GLTFMESHLoader::LoadGLTFModel(std::string(RESOURCES_PATH) + "GLTFMODEL/MIXED_MODEL/mix.gltf"))
	{
		std::cout << "Failed to load the sample model!\n";
	}
	if (!GLTFMESHLoader::LoadGLTFModel(std::string(RESOURCES_PATH) + "GLTFMODEL/CUBE/cube.gltf"))
	{
		std::cout << "Failed to load the sample model!\n";
	}
	if (!GLTFMESHLoader::LoadGLTFModel(std::string(RESOURCES_PATH) + "GLTFMODEL/AVOCADO/Avocado.gltf"))
	{
		std::cout << "Failed to load the sample model!\n";
	}
	if (!GLTFMESHLoader::LoadGLTFModel(std::string(RESOURCES_PATH) + "GLTFMODEL/FISH/BarramundiFish.gltf"))
	{
		std::cout << "Failed to load the sample model!\n";
	}

	// ######### Setting Up The ECS      ##########

	std::shared_ptr<ECSWorld> ecsWorld = std::make_shared<ECSWorld>();
	ecsWorld->InitECSWorld();

	// ############################################

	// ######### Setting Up The Renderer ##########

	std::shared_ptr<MeshRendererSystem> meshRenderSystem = std::make_shared<MeshRendererSystem>(
		(std::string(RESOURCES_PATH) + "SHADER/GLTF_MODEL/gltf_vert.glsl"),
		(std::string(RESOURCES_PATH) + "SHADER/GLTF_MODEL/gltf_frag.glsl"),
		ecsWorld->GetWorld()
	);
	meshRenderSystem->InitMeshRendererSystem();

	// ############################################


	// ##          ##

	flecs::entity e1 = ecsWorld->CreateEntity("Test1");
	e1
		.set<TransfromComponent>({ glm::vec3(3.f, 1.f, 0.f)
		, glm::vec3(0.f, 0.f, 0.f), glm::vec3(50.f, 50.f, 50.f) })
		.set<MeshComponent>({ "Avocado.gltf" });

	flecs::entity e2 = ecsWorld->CreateEntity("Test2");
	e2
		.set<TransfromComponent>({ glm::vec3(0.f, 0.f, 0.f)
		, glm::vec3(0.f, 0.f, 0.f), glm::vec3(50.f, 50.f, 50.f) })
		.set<MeshComponent>({ "Avocado.gltf" });

	// ## Testing some Scripting Stuff ##

	// ## Testing some Scripting Stuff ##

	std::shared_ptr<NativeCPPScriptManager> nativeCPPScriptManager
		= std::make_shared<NativeCPPScriptManager>(SCRIPT_PATH + std::string("ScriptImpl.dll"));
	nativeCPPScriptManager->LoadDependencies(ecsWorld);
	nativeCPPScriptManager->LoadDLL();

	auto world = ecsWorld->GetWorld();

	world->defer_begin();  // start deferring table modifications

	world->query<TransfromComponent>().each([](flecs::entity e, TransfromComponent& p) {
		e.destruct();  // this is deferred safely
	});

	world->defer_end(); // apply all deferred operations
	// Again, if this is not inside a system, you might need a world progress call.
	// flecsWorld->progress();

	while (!Window::shouldClose())
	{
		Window::clearScreen();
		Window::processInput();

		processKeyInput(Window::getGLFWWindow()); 

		// Create projection matrices [PERSPECTIVE]
		glm::mat4 projectionP = glm::mat4(1.0f);
		projectionP = glm::perspective(glm::radians(45.0f), (float)640 / (float)480, 0.1f, 200.0f);

		// Camera or View transformation
		glm::mat4 view = camera.GetViewMatrix();
		shader3.setMat4("view", view);

		meshRenderSystem->MeshRendererUpdate(view, projectionP);

		nativeCPPScriptManager->UpdateScript();

		Window::update();
	}

	Window::cleanup();
}

void processKeyInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
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

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
	static bool firstMouse = true;
	static float lastX = 0.0f;
	static float lastY = 0.0f;

	if (firstMouse)
	{
		lastX = xposIn;
		lastY = yposIn;
		firstMouse = false;
	}

	float deltaX = xposIn - lastX;
	float deltaY = lastY - yposIn;

	lastX = xposIn;
	lastY = yposIn;

	// Right click = Orbit
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
	{
		camera.OnMouseRotate(deltaX, deltaY);
	}
	// Middle click = Pan
	else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS)
	{
		camera.OnMousePan(deltaX, deltaY);
	}
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.OnMouseScroll((float)yoffset);
}

