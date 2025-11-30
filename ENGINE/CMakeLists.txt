add_library(2025ENGINE SHARED
    "GlobalInformation/GlobalInformation.cpp"
    "GlobalInformation/GlobalInformation.h"
    "WINDOW/Window.h" "WINDOW/Window.cpp"
    "PROFILING/Profiler.h" 
    "UTILITIES/Time.h" 
	"UTILITIES/Time.cpp" 
    "CAMERA/Camera.h" 
	"CAMERA/Camera.cpp" 
    "CAMERA/EditorCamera.h" "CAMERA/EditorCamera.cpp"
    "RENDERER/MESH/GLTFMESH/GLTFMESHRenderer.h" 
	"RENDERER/MESH/GLTFMESH/GLTFMESHRenderer.cpp" 
	"RENDERER/MESH/GLTFMESH/GLTFMESHLoader.h" 
	"RENDERER/MESH/GLTFMESH/GLTFMESHLoader.cpp"
    "RENDERER/TEXTURE_KTX/TextureLoader.h" 
	"RENDERER/TEXTURE_KTX/TextureLoader.cpp" 
    "RENDERER/TEXTURE_KTX/TextureKTX2.h"
	"RENDERER/TEXTURE_KTX/TextureKTX2.cpp"
	"RENDERER/SHADERS/Shader.h" 
	"RENDERER/SHADERS/Shader.cpp" 
    "ECS/ECSTest.h" "ECS/ECSTest.cpp" 
	"ECS/COMPONENTS/TransfromComponent.h" 
	"ECS/COMPONENTS/TransfromComponent.cpp" 
	"ECS/COMPONENTS/MeshComponent.h" 
	"ECS/COMPONENTS/MeshComponent.cpp"
	"ECS/ECSWorld.h" 
	"ECS/ECSWorld.cpp" 
	"ECS/SYSTEMS/MeshRendererSystem.h" 
	"ECS/SYSTEMS/MeshRendererSystem.cpp" 
	"SCRIPTING/NativeCPP/NativeCPPGlobalScript.h"
    "SCRIPTING/NativeCPP/NativeCPPScriptManager.h"
    "SCRIPTING/NativeCPP/NativeCPPScriptManager.cpp"
    "RENDERER/MESH/GLTFMESH/GLTFMESHSkeletalAnimationLoader.h" 
    "RENDERER/MESH/GLTFMESH/GLTFMESHSkeletalAnimationLoader.cpp" 
    "ECS/COMPONENTS/AnimationComponent.h" 
    "ECS/COMPONENTS/AnimationComponent.cpp" 
    "INPUT/Button.h" 
    "INPUT/Keys.h" 
    "INPUT/Keys.cpp"  
    "INPUT/KeyBoard.h" 
    "INPUT/KeyBoard.cpp" 
    "INPUT/MouseButtons.h" 
    "INPUT/MouseButtons.cpp" 
    "INPUT/Mouse.h" 
    "INPUT/Mouse.cpp"  
    "DEBUGUI/MicroUI.h" 
    "DEBUGUI/MicroUI.cpp" 
     "DEBUGUI/MicroUIRenderer.h" "DEBUGUI/MicroUIRenderer.cpp")

target_include_directories(2025ENGINE PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}"
)

target_link_libraries(2025ENGINE PUBLIC
    opengl32
    glfw
    glad
    glm::glm-header-only
    ktx
    tinygltf
    reactphysics3d
    flecs
)

# Ensure DLL exports work properly
target_compile_definitions(2025ENGINE PRIVATE ENGINE_EXPORTS)


