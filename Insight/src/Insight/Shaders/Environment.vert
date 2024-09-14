#version 450

layout(location = 0) in vec3 InPosition;
layout(location = 1) in vec3 InNormal;
layout(location = 2) in vec3 InTangent;
layout(location = 3) in vec3 InBitangent;
layout(location = 4) in vec2 InTexCoord;

layout(location = 0) out vec3 OutFragPos;

#include "includes/Scene.glsl"

void main()
{
    OutFragPos = InPosition;

    mat4 rotView = mat4(mat3(Scene.View)); // remove translation from the view matrix

    vec4 clipPos = Scene.Projection * rotView * vec4(InPosition, 1.0);

    gl_Position = clipPos.xyww;
}