#ifndef SAMPLE_SPHERICAL_MAP_GLSL
#define SAMPLE_SPHERICAL_MAP_GLSL

const vec2 SampleSphericalMap_invAtan = vec2(0.1591, 0.3183);

vec2 SampleSphericalMap(vec3 v)
{
    vec2 uv = vec2(atan(v.z, v.x), asin(v.y));
    uv *= SampleSphericalMap_invAtan;
    uv += 0.5;
    return uv;
}

#endif