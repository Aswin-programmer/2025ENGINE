#include "MeshRendererSystem.h"

MeshRendererSystem::MeshRendererSystem()
{

}

MeshRendererSystem::MeshRendererSystem(std::string vertPath, std::string fragPath
	, std::shared_ptr<flecs::world> ecsWorld_)
	:
	vertexShaderPath{vertPath},
	fragmentShaderPath{fragPath},
	gltfMeshRenderer{ std::make_shared<GLTFMESHRenderer>() },
	meshRendererShader{std::make_shared<Shader>(vertPath.c_str(), fragPath.c_str())},
	ecsWorld{ecsWorld_}
{
}

MeshRendererSystem::~MeshRendererSystem()
{
}

void MeshRendererSystem::InitMeshRendererSystem()
{
	meshRendererShader->SetupDefaultTextureSlots();
	meshRendererQuery = ecsWorld->query<TransfromComponent, MeshComponent>();
}

void MeshRendererSystem::MeshRendererUpdate(glm::mat4 view, glm::mat4 proj) {
    gltfMeshRenderer->CleanUp();
    meshRendererQuery.each([&](flecs::entity e, TransfromComponent& transform, MeshComponent& mesh) {
        gltfMeshRenderer->AddGLTFModelToRenderer(
            mesh.GetMeshName(),
            GLTFModelOrientation(
                transform.GetPosition(),
                transform.GetRotation(),
                transform.GetScale(),
                -1
            )
        );
    });

    gltfMeshRenderer->ExperimentalHelper();
    meshRendererShader->use();
    meshRendererShader->setMat4("view", view);
    meshRendererShader->setMat4("projection", proj);
    gltfMeshRenderer->GLTFMESHRender();

}
