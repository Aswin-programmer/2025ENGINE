#version 460 core

in GS_OUT {
    float life;
    float maxLife;
    vec2 texCoord;
} fs_in;

out vec4 FragColor;

void main()
{
    float lifeRatio = clamp(fs_in.life / fs_in.maxLife, 0.0, 1.0);

    //// Create a soft circular alpha mask
    //float dist = length(fs_in.texCoord - vec2(0.5));
    //if (dist > 0.5)
    //    discard;

    vec3 color = mix(vec3(1.0, 0.3, 0.0), vec3(1.0, 1.0, 0.0), lifeRatio);
    FragColor = vec4(color, lifeRatio);
}
