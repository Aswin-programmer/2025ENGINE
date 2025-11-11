#version 460 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;

struct MeshOrientation {
    vec4 Position;
    vec4 Rotation; // Euler angles in degrees
    vec3 Scale;
    int materialIndex;
};

layout(std430, binding = 0) buffer Orientations {
    MeshOrientation meshOrientations[];
};

uniform mat4 view;
uniform mat4 projection;
uniform uint u_NumInstances; // defensive bound check

out vec2 vTexCoord;
out vec3 vNormal;
flat out int materialIndex;

mat4 translate(vec3 t) {
    // Column-major ordering (last column = translation)
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
        0.0, c,  -s,  0.0,
        0.0, s,   c,  0.0,
        0.0, 0.0, 0.0, 1.0
    );
}

mat4 rotateY(float angle) {
    float s = sin(angle);
    float c = cos(angle);
    return mat4(
         c, 0.0, s, 0.0,
        0.0, 1.0, 0.0, 0.0,
        -s, 0.0, c, 0.0,
        0.0, 0.0, 0.0, 1.0
    );
}

mat4 rotateZ(float angle) {
    float s = sin(angle);
    float c = cos(angle);
    return mat4(
        c, -s, 0.0, 0.0,
        s,  c, 0.0, 0.0,
        0.0, 0.0, 1.0, 0.0,
        0.0, 0.0, 0.0, 1.0
    );
}

mat4 composeModelMatrix(MeshOrientation m) {
    mat4 T = translate(m.Position.xyz);
    float radX = radians(m.Rotation.x);
    float radY = radians(m.Rotation.y);
    float radZ = radians(m.Rotation.z);
    mat4 R = rotateZ(radZ) * rotateY(radY) * rotateX(radX);
    mat4 S = scale(m.Scale.xyz);
    return T * R * S;
}

void main()
{
    uint instanceIndex = gl_InstanceID + gl_BaseInstance;
    //if (instanceIndex >= u_NumInstances) {
    //    // fallback, prevents reading garbage from SSBO
    //    instanceIndex = 0u;
    //}

    MeshOrientation orientation = meshOrientations[instanceIndex];

    materialIndex = orientation.materialIndex;

    mat4 model = composeModelMatrix(orientation);

    gl_Position = projection * view * model * vec4(aPos, 1.0);

    vTexCoord = aTexCoord;

    mat3 normalMatrix = transpose(inverse(mat3(model)));
    vNormal = normalize(normalMatrix * aNormal);
}
