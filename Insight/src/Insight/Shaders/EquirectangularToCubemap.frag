#version 450

#include "includes/SampleSphericalMap.glsl"

layout(set = 0, binding = 0) uniform sampler2D InEquirectangularMap;

layout(location = 0) in vec3 InFragPos;

layout(location = 0) out vec4 OutColor;

void main() {
    vec2 uv = SampleSphericalMap(normalize(InFragPos));
//    uv.y = 1.0 - uv.y;
    vec3 color = texture(InEquirectangularMap, uv).rgb;

//    OutColor = vec4(color / (1.0 + color), 1.0);
    color = color / (color + 0.9);

    OutColor = vec4(color, 1.0);
}