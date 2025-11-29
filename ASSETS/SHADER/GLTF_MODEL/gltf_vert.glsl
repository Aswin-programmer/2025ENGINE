#version 460 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;
layout(location = 3) in ivec4 aBones;
layout(location = 4) in vec4 aWeights;

struct MeshOrientation {
    vec4 Position;
    vec4 Rotation; // Euler angles in degrees
    vec3 Scale;
    int materialIndex;
    int animationIndex;
    int temp[3]; // Tempory (padding)
};

layout(std430, binding = 0) buffer Orientations {
    MeshOrientation meshOrientations[];
};

struct GLTFAnimations {
    mat4 jointMatrix[20];
};

layout(std430, binding = 2) buffer Animations {
    GLTFAnimations gltfAnimations[];
};

uniform mat4 view;
uniform mat4 projection;
uniform uint u_NumInstances; // defensive bound check

out vec2 vTexCoord;
out vec3 vNormal;
flat out int materialIndex;

mat4 translate(vec3 t) {
    return mat4(
        1.0, 0.0, 0.0, 0.0,
        0.0, 1.0, 0.0, 0.0,
        0.0, 0.0, 1.0, 0.0,
        t.x, t.y, t.z, 1.0
    );
}

mat4 scaleM(vec3 s) {
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
    mat4 S = scaleM(m.Scale.xyz);
    return T * R * S;
}

void main()
{
    uint instanceIndex = gl_InstanceID + gl_BaseInstance;

    // defensive check (optional, but avoids undefined reads)
    //if (instanceIndex >= u_NumInstances) {
    //    // Render something safe: fallback to instance 0
    //    instanceIndex = 0u;
    //}

    MeshOrientation orientation = meshOrientations[instanceIndex];
    materialIndex = orientation.materialIndex;
    mat4 model = composeModelMatrix(orientation);

    // Skinning support
    mat4 skinMatrix = mat4(0.0);
    bool doSkin = (orientation.animationIndex >= 0);

    if (doSkin) {
        // copy joint matrices from SSBO entry into a local array (or access directly)
        // safer: read directly from gltfAnimations[orientation.animationIndex].jointMatrix[i]
        // compute skinMatrix = sum(weight_i * jointMatrix[bone_i])
        // guard indices and weights:
        ivec4 bones = aBones;
        vec4 weights = aWeights;

        // Optionally normalize weights if they don't sum to 1:
        float wsum = weights.x + weights.y + weights.z + weights.w;
        if (wsum > 0.0) weights /= wsum;

        // accumulate (guard indices)
        for (int i = 0; i < 4; ++i) {
            int boneIndex = bones[i];
            float w = weights[i];
            if (w <= 0.0) continue;
            if (boneIndex >= 0 && boneIndex < 20) {
                skinMatrix += w * gltfAnimations[orientation.animationIndex].jointMatrix[boneIndex];
            }
        }
    } else {
        // skinMatrix = identity if no skinning
        skinMatrix = mat4(1.0);
    }

    // Apply skinning then model transform
    vec4 localPos = vec4(aPos, 1.0);
    vec4 skinnedPos = skinMatrix * localPos;
    vec4 worldPos = model * skinnedPos;
    gl_Position = projection * view * worldPos;

    vTexCoord = aTexCoord;

    // Normals: if skinned, use the upper-left 3x3 of (model * skinMatrix)
    mat3 normalMatrix = mat3(transpose(inverse(model * skinMatrix)));
    vNormal = normalize(normalMatrix * aNormal);
}
