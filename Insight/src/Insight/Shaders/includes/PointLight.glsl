#ifndef POINTLIGHT_GLSL_
#define POINTLIGHT_GLSL_

#include "PBRLighting.glsl"

vec3 Lighting_PointLight(PointLight light, vec3 positionViewSpace, mat4 view, vec3 albedo, vec3 N, float metallic, float roughness) {
    float range = light.PositionRange.a;
    vec3 lightPosition = light.PositionRange.xyz;
    vec3 color = light.ColorIntensity.rgb;
    float intensity = light.ColorIntensity.a;

    vec3 lightPositionViewSpace = vec3(view * vec4(lightPosition, 1.0));

    vec3 V = normalize(-positionViewSpace);
    vec3 L = normalize(lightPositionViewSpace.xyz - positionViewSpace);

    float dist = length(lightPositionViewSpace.xyz - positionViewSpace);
    float attenuation = clamp(1.0 - dist * dist / (range * range), 0.0, 1.0);

    return PBRLighting_LightContribution(V, L, N, albedo, metallic, roughness, color, intensity, attenuation);


    //    if (light.CastsShadows == 1) {
    //        if (dot(N, L) < 0.0) {
    //            shadow = 1;
    //        } else {
    //            shadow = ShadowCalculation(SceneData.PointLights[i], positionViewSpace);
    //        }
    //    }

}

#endif