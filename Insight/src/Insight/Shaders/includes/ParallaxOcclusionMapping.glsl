#ifndef PARALLAX_OCCLUSION_MAPPING_GLSL_
#define PARALLAX_OCCLUSION_MAPPING_GLSL_

vec2 ParallaxOcclusionMapping(sampler2D heightMap, vec2 texCoords, vec3 viewDir) {
    float height_scale = 0.1;

    // texCoords -= (-height_scale, height_scale);

    // number of depth layers
    const float minLayers = 4.0;
    const float maxLayers = 16.0;
    float numLayers =
    mix(maxLayers, minLayers, max(dot(vec3(0.0, 0.0, 1.0), viewDir), 0.0));
    // calculate the size of each layer
    float layerDepth = 1.0 / numLayers;
    // depth of current layer
    float currentLayerDepth = 0.0;
    // the amount to shift the texture coordinates per layer (from vector P)
    vec2 P = -viewDir.xy * height_scale;
    vec2 deltaTexCoords = P / numLayers;

    // get initial values
    vec2 currentTexCoords = texCoords;
    float currentDepthMapValue =
    1.0 - texture(heightMap, currentTexCoords).r;

    while (currentLayerDepth < currentDepthMapValue) {
        // shift texture coordinates along direction of P
        currentTexCoords -= deltaTexCoords;
        // get depthmap value at current texture coordinates
        currentDepthMapValue =
        1.0 - texture(heightMap, currentTexCoords).r;
        // get depth of next layer
        currentLayerDepth += layerDepth;
    }

    // get texture coordinates before collision (reverse operations)
    vec2 prevTexCoords = currentTexCoords + deltaTexCoords;

    // get depth after and before collision for linear interpolation
    float afterDepth = currentDepthMapValue - currentLayerDepth;
    float beforeDepth =
    1.0 - texture(heightMap, prevTexCoords).r -
    currentLayerDepth + layerDepth;

    // interpolation of texture coordinates
    float weight = afterDepth / (afterDepth - beforeDepth);
    vec2 finalTexCoords =
    prevTexCoords * weight + currentTexCoords * (1.0 - weight);

    return finalTexCoords;
}

#endif