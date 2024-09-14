#ifndef GBUFFER_FRAG_IO_GLSL_
#define GBUFFER_FRAG_IO_GLSL_

layout(location = 0) in vec3 FragPos;
layout(location = 1) in vec2 FragTexCoord;
layout(location = 2) in vec3 FragNormal;
layout(location = 3) in mat3 FragTBN;

layout(location = 0) out vec4 OutPosition;
layout(location = 1) out vec4 OutNormal;
layout(location = 2) out vec4 OutOrm;
layout(location = 3) out vec4 OutAlbedo;

#endif