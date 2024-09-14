#version 450

#include "includes/FXAA.glsl"

layout(set = 0, binding = 0) uniform sampler2D InLightingSampler;
layout(set = 1, binding = 0) uniform sampler2D InBloomSampler;

layout(location = 0) in vec2 InFragCoord;

layout(location = 0) out vec4 OutColor;

void main() {
    OutColor = vec4(mix(FXAA(InLightingSampler, InFragCoord).xyz, texture(InBloomSampler, InFragCoord).xyz, 0.005), 1.0);
}
