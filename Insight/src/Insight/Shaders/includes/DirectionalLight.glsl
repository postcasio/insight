#ifndef DIRECTIONALLIGHT_GLSL
#define DIRECTIONALLIGHT_GLSL

#include "PBRLighting.glsl"

vec3 Lighting_DirectionalLight(DirectionalLight light, vec3 positionViewSpace, mat4 view, vec3 albedo, vec3 N, float metallic, float roughness) {
    vec3 lightDirection = light.Direction.xyz;
    vec3 color = light.ColorIntensity.rgb;
    float intensity = light.ColorIntensity.a;

    vec3 lightDirectionViewSpace = vec3(view * vec4(lightDirection, 1.0));

    vec3 V = normalize(-positionViewSpace);
    vec3 L = -normalize(lightDirectionViewSpace.xyz);

    return PBRLighting_LightContribution(V, L, N, albedo, metallic, roughness, color, intensity, 1.0);


    //    if (light.CastsShadows == 1) {
    //        if (dot(N, L) < 0.0) {
    //            shadow = 1;
    //        } else {
    //            shadow = ShadowCalculation(SceneData.PointLights[i], positionViewSpace);
    //        }
    //    }

}

#endif