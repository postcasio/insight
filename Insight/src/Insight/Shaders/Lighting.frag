#version 450

#include "includes/Scene.glsl"
#include "includes/GBufferSamplers.glsl"
#include "includes/PointLight.glsl"
#include "includes/DirectionalLight.glsl"

layout(location = 0) in vec2 InFragCoord;

layout(location = 0) out vec4 OutColor;

void main() {
    vec3 Lo = vec3(0, 0, 0);
    vec3 normal = normalize(texture(NormalSampler, InFragCoord).xyz);
    vec3 position = texture(PositionSampler, InFragCoord).xyz;
    vec3 albedo = texture(AlbedoSampler, InFragCoord).xyz;
    vec3 orm = texture(OrmSampler, InFragCoord).xyz;
    float metallic = orm.x;
    float roughness = orm.y;

    for (int i = 0; i < Scene.PointLightCount; i++) {
        Lo += Lighting_PointLight(Scene.PointLights[i], position, Scene.View, albedo, normal, metallic, roughness);
    }

    for (int i = 0; i < Scene.DirectionalLightCount; i++) {
        Lo += Lighting_DirectionalLight(Scene.DirectionalLights[i], position, Scene.View, albedo, normal, metallic, roughness);
    }

    vec3 ambient;

    vec3 F0 = mix(vec3(0.04), albedo, metallic);

    if (Scene.HasEnvironment) {
        vec3 V = normalize(-position);
        vec3 R = vec3(vec4(reflect(-V, normal), 0) * Scene.View);

        const float MAX_REFLECTION_LOD = textureQueryLevels(InPrefilteredEnvironmentMap).x - 1;
        vec3 prefilteredColor = textureLod(InPrefilteredEnvironmentMap, R, roughness * MAX_REFLECTION_LOD).rgb;

        vec3 F        = fresnelSchlickRoughness(max(dot(normal, V), 0.0), F0, roughness);
        vec2 envBRDF  = texture(InBrdfLut, vec2(max(dot(normal, V), 0.0), roughness)).rg;
        vec3 specular = prefilteredColor * (F * envBRDF.x + envBRDF.y);

        vec3 kS = F;
        vec3 kD = 1.0 - kS;
        vec3 irradiance = texture(InIrradianceMap, vec3(vec4(normal, 0) * Scene.View)).rgb;
        vec3 diffuse = irradiance * albedo;

//        ambient = (kD * diffuse) * orm.x;
        ambient = (kD * diffuse + specular) * orm.x;
    } else {
        ambient = vec3(0.05, 0.05, 0.05) * albedo * orm.x;
    }

    Lo += ambient;

    OutColor = vec4(Lo, 1.0);
}
