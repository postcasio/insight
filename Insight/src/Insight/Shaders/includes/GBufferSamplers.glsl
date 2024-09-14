#ifndef GBUFFER_SAMPLERS_GLSL_
#define GBUFFER_SAMPLERS_GLSL_

layout(set = 1, binding = 0) uniform sampler2D PositionSampler;
layout(set = 1, binding = 1) uniform sampler2D NormalSampler;
layout(set = 1, binding = 2) uniform sampler2D OrmSampler;
layout(set = 1, binding = 3) uniform sampler2D AlbedoSampler;

#endif