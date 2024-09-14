#version 450

layout(location = 0) out vec2 inFragCoord;

vec2 positions[] = vec2[](vec2(-1.0, -1.0), vec2(3.0, -1.0), vec2(-1.0, 3.0));
vec2 uvs[] = vec2[](vec2(0.0, 0.0), vec2(2.0, 0.0), vec2(0.0, 2.0));

void main() {
    gl_Position = vec4(positions[gl_VertexIndex], 0.0, 1.0);

    inFragCoord = uvs[gl_VertexIndex];
}