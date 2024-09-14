#ifndef SCENE_GLSL
#define SCENE_GLSL

struct PointLight {
    vec4 PositionRange;
    vec4 ColorIntensity;
    int CastsShadows;
};

struct DirectionalLight {
    vec4 Direction;
    vec4 ColorIntensity;
    int CastsShadows;
};

struct PostProcess {
    float Exposure;
    float Bloom;
};

layout(std140, set = 0, binding = 0) uniform SceneUniform {
    mat4 Projection;
    mat4 View;
    vec3 CameraPos;
    float NearPlane;
    float FarPlane;
    float Time;

    bool HasEnvironment;

    PointLight PointLights[16];
    int PointLightCount;

    DirectionalLight DirectionalLights[4];
    int DirectionalLightCount;

    PostProcess PostProcess;
} Scene;

layout(set = 0, binding = 1) uniform samplerCube InIrradianceMap;
layout(set = 0, binding = 2) uniform samplerCube InPrefilteredEnvironmentMap;
layout(set = 0, binding = 3) uniform sampler2D InBrdfLut;

#endif