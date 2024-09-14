#version 450

layout(set = 0, binding = 0) uniform sampler2D InFinalRender;
layout(set = 0, binding = 1) uniform sampler2D InUi;

layout(location = 0) in vec2 InFragCoord;

layout(location = 0) out vec4 OutColor;

layout(std140, push_constant) uniform PushConstants {
    bool HasFinalRender;
    bool HasUi;
} CompositorOptions;

vec3 tonemap(vec3 x) {
    return x / (x + 0.155) * 1.019;
}

void main() {
    vec4 finalColor = CompositorOptions.HasFinalRender ? texture(InFinalRender, InFragCoord) : vec4(0.0, 0.0, 0.0, 1.0);
    vec4 uiColor = CompositorOptions.HasUi ? texture(InUi, InFragCoord) : vec4(0.0, 0.0, 0.0, 0.0);


    vec4 mixColor = mix(finalColor, uiColor, uiColor.a);

    OutColor = vec4(tonemap(mixColor.rgb), 1.0);

}