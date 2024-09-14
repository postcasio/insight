#ifndef GBUFFER_VERT_IO_GLSL_
#define GBUFFER_VERT_IO_GLSL_

layout(location = 0) in vec3 InPosition;
layout(location = 1) in vec3 InNormal;
layout(location = 2) in vec3 InTangent;
layout(location = 3) in vec3 InBitangent;
layout(location = 4) in vec2 InTexCoord;

layout(location = 0) out vec3 FragPos;
layout(location = 1) out vec2 FragTexCoord;
layout(location = 2) out vec3 FragNormal;
layout(location = 3) out mat3 FragTBN;

#endif