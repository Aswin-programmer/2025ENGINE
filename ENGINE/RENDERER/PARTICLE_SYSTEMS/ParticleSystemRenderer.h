#pragma once

#include <iostream>
#include <cstddef>
#include <vector>
#include <set>
#include <unordered_set>
#include <unordered_map>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../SHADERS/Shader.h"
#include "../../CAMERA/Camera.h"
#include "../TEXTURE_KTX/TextureLoader.h"

constexpr int MAX_PARTICLES = 100;
constexpr glm::vec3 GRAVITY = glm::vec3(0.0f, -0.98, 0.0f);
constexpr glm::vec3 ORIGIN = glm::vec3(0.0f);

struct Particle
{
	Particle()
		:
		position(0.0f), velocity(0.0f), life(0.0f), maxLife(1.0f)
	{

	}

	glm::vec3 position;
	glm::vec3 velocity;
	float life;
	float maxLife;
};

class ParticleSystemRenderer
{
public:
	ParticleSystemRenderer();
	~ParticleSystemRenderer();

	void CleanUp();
	void RefreshParticles(float dt);
	void Render();
private:
	GLuint particleVAO, particleVBO;

	std::vector<Particle> particles;

	void SetupBuffers();
};