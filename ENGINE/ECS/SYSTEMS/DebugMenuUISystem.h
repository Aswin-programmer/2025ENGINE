#pragma once

#ifdef ENGINE_EXPORTS
#define ENGINE_API __declspec(dllexport)
#else
#define ENGINE_API __declspec(dllimport)
#endif

#include <memory>
#include <iostream>

#include <flecs.h>

#include "../../ECS/COMPONENTS/TransfromComponent.h"
#include "../../ECS/COMPONENTS/MeshComponent.h"
#include "../../ECS/COMPONENTS/AnimationComponent.h"
#include "../../ECS/COMPONENTS/LightingComponent.h"

#include "../../DEBUGUI/MicroUI.h"

class ENGINE_API DebugMenuUISystem
{
public:
    DebugMenuUISystem() = default;
    DebugMenuUISystem(mu_Context* ctx, std::shared_ptr<flecs::world> ecsWorld);
    ~DebugMenuUISystem();
    
    void InitDebugMenuUISystem();

    void StartRenderMenuUISystem();
    void RenderUIMenu();
    void PerformanceUIMenu();
    void EntityManagerMenu();
    void EndRenderMenuUISystem();

private:
    mu_Context* ctx = nullptr;
    std::shared_ptr<flecs::world> ecsWorld;

    flecs::query<TransfromComponent, MeshComponent> staticMeshRendererQuery;
    flecs::query<TransfromComponent, MeshComponent, AnimationComponent> animatedMeshRendererQuery;
    flecs::query<TransfromComponent, LightingComponent> lightingQuery;
};


