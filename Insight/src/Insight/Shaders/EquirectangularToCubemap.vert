#version 450

layout(location = 0) in vec3 InPosition;
layout(location = 1) in vec3 InNormal;
layout(location = 2) in vec3 InTangent;
layout(location = 3) in vec3 InBitangent;
layout(location = 4) in vec2 InTexCoord;

layout(location = 0) out vec3 OutFragPos;

layout(push_constant) uniform ParametersUniform {
    mat4 Projection;
    mat4 View;

} Parameters;

void main()
{
    OutFragPos = InPosition;

    gl_Position = Parameters.Projection * Parameters.View * vec4(InPosition, 1.0);
}