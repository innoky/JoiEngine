#version 450

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec3 fragPos;
layout(location = 2) in vec2 fragUV;

layout(location = 0) out vec4 outColor;

layout(binding = 1) uniform sampler2D texSampler;

void main() {
    vec3 texColor = texture(texSampler, fragUV).rgb;
    float lighting = 0.2 + 0.8 * abs(normalize(fragPos).y);
    outColor = vec4(texColor * lighting, 1.0);
}
