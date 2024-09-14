#version 450
// This shader performs downsampling on a texture,
// as taken from Call Of Duty method, presented at ACM Siggraph 2014.
// This particular method was customly designed to eliminate
// "pulsating artifacts and temporal stability issues".

// Remember to add bilinear minification filter for this texture!
// Remember to use a floating-point texture format (for HDR)!
// Remember to use edge clamping for this texture!
layout(set = 0, binding = 0) uniform sampler2D inTexture;

layout(location = 0) in vec2 inTexCoord;

layout(location = 0) out vec4 outColor;

void main() {
    float filterRadius = 0.005 / textureSize(inTexture, 0).x / 512.0;

    // The filter kernel is applied with a radius, specified in texture
    // coordinates, so that the radius will vary across mip resolutions.
    ivec2 texSize = textureSize(inTexture, 0);

    float x = filterRadius;
    float y = filterRadius * float(texSize.x) / float(texSize.y);

    // Take 9 samples around current texel:
    // a - b - c
    // d - e - f
    // g - h - i
    // === ('e' is the current texel) ===
    vec3 a = texture(inTexture, vec2(inTexCoord.x - x, inTexCoord.y + y)).rgb;
    vec3 b = texture(inTexture, vec2(inTexCoord.x, inTexCoord.y + y)).rgb;
    vec3 c = texture(inTexture, vec2(inTexCoord.x + x, inTexCoord.y + y)).rgb;

    vec3 d = texture(inTexture, vec2(inTexCoord.x - x, inTexCoord.y)).rgb;
    vec3 e = texture(inTexture, vec2(inTexCoord.x, inTexCoord.y)).rgb;
    vec3 f = texture(inTexture, vec2(inTexCoord.x + x, inTexCoord.y)).rgb;

    vec3 g = texture(inTexture, vec2(inTexCoord.x - x, inTexCoord.y - y)).rgb;
    vec3 h = texture(inTexture, vec2(inTexCoord.x, inTexCoord.y - y)).rgb;
    vec3 i = texture(inTexture, vec2(inTexCoord.x + x, inTexCoord.y - y)).rgb;

    // Apply weighted distribution, by using a 3x3 tent filter:
    //  1   | 1 2 1 |
    // -- * | 2 4 2 |
    // 16   | 1 2 1 |
    vec3 upsample = e * 4.0;
    upsample += (b + d + f + h) * 2.0;
    upsample += (a + c + g + i);
    upsample *= 1.0 / 16.0;

    outColor = vec4(upsample, 1.0);
}