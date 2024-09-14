#version 450

#include "includes/Scene.glsl"
#include "includes/GBufferFragIo.glsl"
#include "includes/UnpackNormal.glsl"

layout(set = 1, binding = 0) uniform sampler2D AlbedoSampler;

void main() {
    OutAlbedo = vec4(texture(AlbedoSampler, FragTexCoord).xyz * 2.0, 1.0);
    OutPosition = vec4(FragPos, 1.0);
    OutNormal = vec4(0.0, 0.0, 1.0, 1.0);
    OutOrm = vec4(1.0, 0.2, 0.0, 1.0);
}