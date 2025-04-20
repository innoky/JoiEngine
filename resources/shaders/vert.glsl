#version 450

layout(location = 0) in vec3 inPos;
layout(location = 1) in vec3 inColor;

layout(location = 0) out vec3 fragColor;
layout(location = 1) out vec3 fragPos; // добавляем

layout(push_constant) uniform PushConstants {
    mat4 model;
    mat4 view;
    mat4 proj;
} pc;

void main() {
    vec4 worldPos = pc.model * vec4(inPos, 1.0); // в мировых координатах
    fragPos = worldPos.xyz;
    fragColor = inColor;
    gl_Position = pc.proj * pc.view * worldPos;
}
