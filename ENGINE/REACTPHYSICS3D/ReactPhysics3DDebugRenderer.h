#pragma once

#ifdef ENGINE_EXPORTS
#define ENGINE_API __declspec(dllexport)
#else
#define ENGINE_API __declspec(dllimport)
#endif

#define MAX_VERTICES 2000

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <reactphysics3d/ReactPhysics3D.h>

#include "ReactPhysics3DCore.h"
#include "../RENDERER/SHADERS/Shader.h"


class ENGINE_API ReactPhysicsDebugRenderer
{
public:
	ReactPhysicsDebugRenderer();
	ReactPhysicsDebugRenderer(
		std::string vertPath, 
		std::string fragPath, 
		std::shared_ptr<ReactPhysics3DCore> reactPhysics3DCore
	);
	~ReactPhysicsDebugRenderer();

	void ReactPhysicsRendererRender(glm::mat4 view, glm::mat4 proj);

private:
	std::shared_ptr<ReactPhysics3DCore> reactPhysics3DCore;

	GLuint debugVAO;
	GLuint debugVBO;

	std::shared_ptr<Shader> debugShader;
};

