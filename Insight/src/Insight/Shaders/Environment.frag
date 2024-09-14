#version 450

#include "includes/Scene.glsl"

layout(std140, set = 1, binding = 0) uniform EnvironmentUniform {
    int HasEnvironmentMap;
} Environment;

layout(set = 1, binding = 1) uniform samplerCube InEnvironmentMap;

layout(location = 0) in vec3 InFragPos;

layout(location = 0) out vec4 OutColor;

void main() {
    vec3 envColor;

    if (Environment.HasEnvironmentMap == 1) {
        envColor = textureLod(InEnvironmentMap, InFragPos, 0).rgb;
    } else {
        envColor = vec3(0);
    }

    OutColor = vec4(envColor, 1.0);
}