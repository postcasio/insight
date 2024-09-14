#version 450

layout(set = 0, binding = 0) uniform samplerCube InCubemap;

layout(location = 0) in vec3 InFragPos;

layout(location = 0) out vec4 OutColor;

layout(push_constant) uniform ParametersUniform {
    mat4 Projection;
    mat4 View;
    float Roughness;
} Parameters;

#include "includes/PBRLighting.glsl"

void main() {
    vec3 N = normalize(InFragPos);
    vec3 R = N;
    vec3 V = R;

    const uint SAMPLE_COUNT = 1024u;
    float totalWeight = 0.0;
    vec3 prefilteredColor = vec3(0.0);
    for(uint i = 0u; i < SAMPLE_COUNT; ++i)
    {
        vec2 Xi = Hammersley(i, SAMPLE_COUNT);
        vec3 H  = ImportanceSampleGGX(Xi, N, Parameters.Roughness);
        vec3 L  = normalize(2.0 * dot(V, H) * H - V);

        float NdotL = max(dot(N, L), 0.0);

        if(NdotL > 0.0)
        {
            float NdotH = max(dot(N, H), 0.0);
            float HdotV = max(dot(H, V), 0.0);

            float D   = DistributionGGX(N, H, Parameters.Roughness);
            float pdf = (D * NdotH / (4.0 * HdotV)) + 0.0001;

            float resolution = textureSize(InCubemap, 0).x; // resolution of source cubemap (per face)
            float saTexel  = 4.0 * PI / (6.0 * resolution * resolution);
            float saSample = 1.0 / (float(SAMPLE_COUNT) * pdf + 0.0001);

            float mipLevel = Parameters.Roughness == 0.0 ? 0.0 : 0.5 * log2(saSample / saTexel);

            prefilteredColor += texture(InCubemap, L).rgb * NdotL;
            totalWeight      += NdotL;
        }
    }
    prefilteredColor = prefilteredColor / totalWeight;

    OutColor = vec4(prefilteredColor, 1.0);
}