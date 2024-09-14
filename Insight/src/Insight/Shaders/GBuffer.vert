#version 450

#include "includes/Scene.glsl"
#include "includes/Model.glsl"
#include "includes/GBufferVertIo.glsl"

void main() {
    gl_Position = Scene.Projection * Scene.View * Model.Transform * vec4(InPosition, 1.0);

    FragTexCoord = InTexCoord;
    FragPos = vec3(Scene.View * Model.Transform * vec4(InPosition, 1.0));

    vec3 tangent = normalize(InTangent);
    vec3 normal = normalize(InNormal);
    vec3 bitangent = normalize(InBitangent);

    FragTBN = mat3(Scene.View * Model.Transform) * mat3(tangent, bitangent, normal);
    FragNormal = normalize(vec3(Scene.View * Model.Transform * vec4(InNormal, 1.0)));
}