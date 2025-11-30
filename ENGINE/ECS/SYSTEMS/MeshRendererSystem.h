#pragma once

#ifdef ENGINE_EXPORTS
#define ENGINE_API __declspec(dllexport)
#else
#define ENGINE_API __declspec(dllimport)
#endif

#include <memory>
#include <iostream>

#include <flecs.h>

#include "../../RENDERER/MESH/GLTFMESH/GLTFMESHRenderer.h"
#include "../../RENDERER/MESH/GLTFMESH/GLTFMESHSkeletalAnimationLoader.h"
#include "../../RENDERER/SHADERS/Shader.h"

#include "../../ECS/COMPONENTS/TransfromComponent.h"
#include "../../ECS/COMPONENTS/MeshComponent.h"
#include "../../ECS/COMPONENTS/AnimationComponent.h"

#include "../../Window/Window.h"

#include "../../DEBUGUI/MicroUI.h"

class ENGINE_API MeshRendererSystem
{
public:
    MeshRendererSystem();
    MeshRendererSystem(std::string vertPath, std::string fragPath, std::shared_ptr<flecs::world> ecsWorld_);
    ~MeshRendererSystem();

    void InitMeshRendererSystem();
    void MeshRendererUpdate(glm::mat4 view, glm::mat4 proj);

    void DebugMenu(mu_Context* ctx);

private:
    std::string vertexShaderPath;
    std::string fragmentShaderPath;

    std::shared_ptr<GLTFMESHRenderer> gltfMeshRenderer;
    std::shared_ptr<Shader> meshRendererShader;
    std::shared_ptr<flecs::world> ecsWorld;
    flecs::query<TransfromComponent, MeshComponent> staticMeshRendererQuery;
    flecs::query<TransfromComponent, MeshComponent, AnimationComponent> animatedMeshRendererQuery;
};