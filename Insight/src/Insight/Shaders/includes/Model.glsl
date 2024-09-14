#ifndef MODEL_GLSL
#define MODEL_GLSL

layout(push_constant) uniform ModelUniform {
    mat4 Transform;
    mat4 Normal;
} Model;

#endif