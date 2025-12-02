#version 460 core

vec2 positions[6] = vec2[](
    vec2(-1.0, -1.0),
    vec2( 1.0, -1.0),
    vec2( 1.0,  1.0),
    vec2(-1.0, -1.0),
    vec2( 1.0,  1.0),
    vec2(-1.0,  1.0)
);

out vec2 TexCoord;


void main() {
    TexCoord = (positions[gl_VertexID] + 1.0) * 0.5;
    gl_Position = vec4(positions[gl_VertexID], 0.0, 1.0);
}
