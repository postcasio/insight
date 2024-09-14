#version 450

#include "includes/Scene.glsl"
#include "includes/GBufferFragIo.glsl"
#include "includes/UnpackNormal.glsl"

layout(std140, set = 1, binding = 0) uniform MaterialUniform {
    float Roughness;
} Material;

layout(set = 1, binding = 1) uniform sampler2D AlbedoSampler;
layout(set = 1, binding = 2) uniform sampler2D NormalSampler;
layout(set = 1, binding = 3) uniform sampler2D OrmSampler;

void main() {
    OutAlbedo = texture(AlbedoSampler, FragTexCoord);
    OutPosition = vec4(FragPos, 1.0);
    OutNormal = vec4(UnpackNormal(texture(NormalSampler, FragTexCoord).rgb), 1.0);
    OutOrm = vec4(texture(OrmSampler, FragTexCoord).rgb * vec3(1, Material.Roughness, 1), 1.0);
}