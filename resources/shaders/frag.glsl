#version 450

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec3 fragPos;

layout(location = 0) out vec4 outColor;

void main() {
    float lighting = 0.2 + 0.8 * abs(normalize(fragPos).y); // псевдосвет сверху
    outColor = vec4(fragColor * lighting, 1.0);
}
