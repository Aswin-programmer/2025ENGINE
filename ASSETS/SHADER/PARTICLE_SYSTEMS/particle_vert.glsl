#version 460 core

layout(location = 0) in vec3 aPos;       // Particle position
layout(location = 1) in vec3 aVelocity;  // (optional, unused here)
layout(location = 2) in float aLife;
layout(location = 3) in float aMaxLife;

out VS_OUT
{
    vec3 position;
    float life;
    float maxLife;
}vs_out;

void main()
{
    vs_out.life = aLife;
    vs_out.maxLife = aMaxLife;
    vs_out.position = aPos;
}
