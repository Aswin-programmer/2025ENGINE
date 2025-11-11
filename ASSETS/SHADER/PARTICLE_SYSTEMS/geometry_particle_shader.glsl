// Geometry Shader

#version 460 core
layout(points) in;
layout(triangle_strip, max_vertices = 36) out;

in VS_OUT {
    vec3 position;
    float life;
    float maxLife;
} gs_in[];

out GS_OUT {
    float life;
    float maxLife;
    vec2 texCoord; // optional
} gs_out;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void EmitTriangle(vec3 a, vec3 b, vec3 c)
{
    gs_out.texCoord = vec2(0); // optional
    gl_Position = projection * view * model * vec4(a, 1.0);
    EmitVertex();

    gl_Position = projection * view * model * vec4(b, 1.0);
    EmitVertex();

    gl_Position = projection * view * model * vec4(c, 1.0);
    EmitVertex();

    EndPrimitive();
}

void main()
{
    vec3 center = gs_in[0].position;
    float size = 0.025;
    float hs = size * 0.5;

    // Define the 8 cube corners
    vec3 v[8] = vec3[](
        center + vec3(-hs, -hs, -hs), // 0
        center + vec3( hs, -hs, -hs), // 1
        center + vec3( hs,  hs, -hs), // 2
        center + vec3(-hs,  hs, -hs), // 3
        center + vec3(-hs, -hs,  hs), // 4
        center + vec3( hs, -hs,  hs), // 5
        center + vec3( hs,  hs,  hs), // 6
        center + vec3(-hs,  hs,  hs)  // 7
    );

    gs_out.life = gs_in[0].life;
    gs_out.maxLife = gs_in[0].maxLife;

    // Emit 12 triangles (2 per face)
    // Front face
    EmitTriangle(v[0], v[1], v[2]);
    EmitTriangle(v[2], v[3], v[0]);

    // Back face
    EmitTriangle(v[5], v[4], v[7]);
    EmitTriangle(v[7], v[6], v[5]);

    // Left face
    EmitTriangle(v[4], v[0], v[3]);
    EmitTriangle(v[3], v[7], v[4]);

    // Right face
    EmitTriangle(v[1], v[5], v[6]);
    EmitTriangle(v[6], v[2], v[1]);

    // Top face
    EmitTriangle(v[3], v[2], v[6]);
    EmitTriangle(v[6], v[7], v[3]);

    // Bottom face
    EmitTriangle(v[4], v[5], v[1]);
    EmitTriangle(v[1], v[0], v[4]);
}
