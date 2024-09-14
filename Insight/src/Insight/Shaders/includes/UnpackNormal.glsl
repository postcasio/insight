#ifndef UNPACK_NORMAL_GLSL
#define UNPACK_NORMAL_GLSL

#include "GBufferFragIo.glsl"

vec3 UnpackNormal(vec3 normal) {
    normal = normalize(normal * 2.0 - 1.0);
    return normalize(FragTBN * normal);
}

#endif