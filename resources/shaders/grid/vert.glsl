#version 450

layout(location = 0) in vec3 inPos;

layout(push_constant) uniform PushConstants {
    mat4 model;
    mat4 view;
    mat4 proj;
} pc;

void main() {
    gl_Position = pc.proj * pc.view * pc.model * vec4(inPos, 1.0);
}
