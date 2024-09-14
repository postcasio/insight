void FXAA_texcoords(vec2 fragCoord, vec2 resolution, out vec2 v_rgbNW,
out vec2 v_rgbNE, out vec2 v_rgbSW, out vec2 v_rgbSE,
out vec2 v_rgbM) {
    vec2 inverseVP = 1.0 / resolution.xy;
    v_rgbNW = (fragCoord + vec2(-1.0, -1.0)) * inverseVP;
    v_rgbNE = (fragCoord + vec2(1.0, -1.0)) * inverseVP;
    v_rgbSW = (fragCoord + vec2(-1.0, 1.0)) * inverseVP;
    v_rgbSE = (fragCoord + vec2(1.0, 1.0)) * inverseVP;
    v_rgbM = vec2(fragCoord * inverseVP);
}

vec3 FXAA(sampler2D inHdr, vec2 inTexCoord) {
    vec2 resolution = vec2(textureSize(inHdr, 0));
    vec2 v_rgbNW;
    vec2 v_rgbNE;
    vec2 v_rgbSW;
    vec2 v_rgbSE;
    vec2 v_rgbM;

    FXAA_texcoords(gl_FragCoord.xy, resolution, v_rgbNW, v_rgbNE, v_rgbSW, v_rgbSE,
    v_rgbM);

    vec3 rgbNW = texture(inHdr, v_rgbNW).rgb;
    vec3 rgbNE = texture(inHdr, v_rgbNE).rgb;
    vec3 rgbSW = texture(inHdr, v_rgbSW).rgb;
    vec3 rgbSE = texture(inHdr, v_rgbSE).rgb;
    vec3 rgbM = texture(inHdr, v_rgbM).rgb;

    vec3 luma = vec3(0.299, 0.587, 0.114);
    float lumaNW = dot(rgbNW, luma);
    float lumaNE = dot(rgbNE, luma);
    float lumaSW = dot(rgbSW, luma);
    float lumaSE = dot(rgbSE, luma);
    float lumaM = dot(rgbM, luma);

    float lumaMin = min(lumaM, min(min(min(lumaNW, lumaNE), lumaSW), lumaSE));
    float lumaMax = max(lumaM, max(max(max(lumaNW, lumaNE), lumaSW), lumaSE));

    vec2 dir;
    dir.x = -((lumaNW + lumaNE) - (lumaSW + lumaSE));
    dir.y = ((lumaNW + lumaSW) - (lumaNE + lumaSE));

    float dirReduce = max((lumaNW + lumaNE + lumaSW + lumaSE) * (0.25 * 0.25), 0.0078125);
    float rcpDirMin = 1.0 / (min(abs(dir.x), abs(dir.y)) + dirReduce);
    dir = min(vec2(8.0, 8.0), max(vec2(-8.0, -8.0), dir * rcpDirMin)) / resolution.xy;

    vec3 rgbA = 0.5 * (texture(inHdr, inTexCoord + dir * (1.0 / 3.0 - 0.5)).rgb +
    texture(inHdr, inTexCoord + dir * (2.0 / 3.0 - 0.5)).rgb);
    vec3 rgbB = rgbA * 0.5 + 0.25 * (texture(inHdr, inTexCoord + dir * -0.5).rgb +
    texture(inHdr, inTexCoord + dir * 0.5).rgb);

    float lumaB = dot(rgbB, luma);

    if ((lumaB < lumaMin) || (lumaB > lumaMax)) {
        return rgbA;
    } else {
        return rgbB;
    }
}