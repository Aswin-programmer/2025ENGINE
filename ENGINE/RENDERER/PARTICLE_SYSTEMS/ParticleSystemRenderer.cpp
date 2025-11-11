#include "ParticleSystemRenderer.h"

ParticleSystemRenderer::ParticleSystemRenderer()
	:
	particleVAO{0},
	particleVBO{0},
	particles{std::vector<Particle>(MAX_PARTICLES)}
{
	SetupBuffers();
}

ParticleSystemRenderer::~ParticleSystemRenderer()
{
    particles.clear();
    glDeleteBuffers(1, &particleVBO);
    glDeleteVertexArrays(1, &particleVAO);
}

void ParticleSystemRenderer::CleanUp()
{
    particles.clear();
}

void ParticleSystemRenderer::RefreshParticles(float dt)
{
    for (auto& p : particles) {
        if (p.life > 0.0f) {
            p.velocity += GRAVITY * dt;
            p.position += p.velocity * dt;
            p.life -= dt;
        }
        else {
            // Respawn from origin with new velocity
            p.position = ORIGIN;

            float x = (rand() % 100 - 50) / 50.0f;
            float y = (rand() % 100) / 50.0f + 2.0f;
            float z = (rand() % 100 - 50) / 50.0f;

            p.velocity = glm::vec3(x, y, z);
            p.maxLife = p.life = 1.5f + static_cast<float>(rand() % 100) / 100.0f;
        }
    }
}

void ParticleSystemRenderer::Render()
{
    glBindVertexArray(particleVAO);

    glNamedBufferSubData(particleVBO, 0, sizeof(Particle) * MAX_PARTICLES, particles.data());

    glDrawArrays(GL_POINTS, 0, MAX_PARTICLES);
}

void ParticleSystemRenderer::SetupBuffers()
{
    glCreateVertexArrays(1, &particleVAO);

    glCreateBuffers(1, &particleVBO);
    glNamedBufferStorage(particleVBO, sizeof(Particle) * MAX_PARTICLES, nullptr, GL_DYNAMIC_STORAGE_BIT);

    // Bind buffer to VAO at binding index 0
    glVertexArrayVertexBuffer(particleVAO, 0, particleVBO, 0, sizeof(Particle));

    // position (location = 0)
    glVertexArrayAttribFormat(particleVAO, 0, 3, GL_FLOAT, GL_FALSE, offsetof(Particle, position));
    glVertexArrayAttribBinding(particleVAO, 0, 0);
    glEnableVertexArrayAttrib(particleVAO, 0);

    // velocity (location = 1)
    glVertexArrayAttribFormat(particleVAO, 1, 3, GL_FLOAT, GL_FALSE, offsetof(Particle, velocity));
    glVertexArrayAttribBinding(particleVAO, 1, 0);
    glEnableVertexArrayAttrib(particleVAO, 1);

    // life (location = 2)
    glVertexArrayAttribFormat(particleVAO, 2, 1, GL_FLOAT, GL_FALSE, offsetof(Particle, life));
    glVertexArrayAttribBinding(particleVAO, 2, 0);
    glEnableVertexArrayAttrib(particleVAO, 2);

    // maxLife (location = 3)
    glVertexArrayAttribFormat(particleVAO, 3, 1, GL_FLOAT, GL_FALSE, offsetof(Particle, maxLife));
    glVertexArrayAttribBinding(particleVAO, 3, 0);
    glEnableVertexArrayAttrib(particleVAO, 3);
}