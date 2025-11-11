#version 460 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCoord;

struct MeshOrientation {
    vec4 Position;
    vec4 Rotation; // XYZ in radians
    vec4 Scale;
};

layout(std430, binding = 0) buffer Orientations {
    MeshOrientation meshOrientations[];
};

uniform mat4 view;
uniform mat4 projection;

out vec2 vTexCoord;

// Custom GLSL matrix functions
mat4 translate(vec3 t) {
    return mat4(
        1.0, 0.0, 0.0, 0.0,
        0.0, 1.0, 0.0, 0.0,
        0.0, 0.0, 1.0, 0.0,
         t.x, t.y, t.z, 1.0
    );
}

mat4 scale(vec3 s) {
    return mat4(
        s.x, 0.0, 0.0, 0.0,
        0.0, s.y, 0.0, 0.0,
        0.0, 0.0, s.z, 0.0,
        0.0, 0.0, 0.0, 1.0
    );
}

mat4 rotateX(float angle) {
    float s = sin(angle);
    float c = cos(angle);
    return mat4(
        1.0, 0.0, 0.0, 0.0,
        0.0, c,   s,   0.0,
        0.0, -s,  c,   0.0,
        0.0, 0.0, 0.0, 1.0
    );
}

mat4 rotateY(float angle) {
    float s = sin(angle);
    float c = cos(angle);
    return mat4(
        c,   0.0, -s,  0.0,
        0.0, 1.0, 0.0, 0.0,
        s,   0.0, c,   0.0,
        0.0, 0.0, 0.0, 1.0
    );
}

mat4 rotateZ(float angle) {
    float s = sin(angle);
    float c = cos(angle);
    return mat4(
        c,   s,   0.0, 0.0,
       -s,   c,   0.0, 0.0,
        0.0, 0.0, 1.0, 0.0,
        0.0, 0.0, 0.0, 1.0
    );
}

mat4 composeTransform(MeshOrientation m) {
    mat4 T = translate(m.Position.xyz);
    mat4 R = rotateZ(m.Rotation.z) * rotateY(m.Rotation.y) * rotateX(m.Rotation.x);
    mat4 S = scale(m.Scale.xyz);
    return T * R * S;
}

void main()
{
    uint instanceID = gl_InstanceID;
    MeshOrientation orientation = meshOrientations[instanceID + gl_BaseInstance];
    mat4 model = composeTransform(orientation);

    gl_Position = projection * view * model * vec4(aPos, 1.0);
    vTexCoord = aTexCoord;
}
