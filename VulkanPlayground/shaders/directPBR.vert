#version 450

layout(binding = 0) uniform UniformBufferObject {
    mat4 model;
    mat4 view;
    mat4 proj;
} ubo;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;

layout(location = 0) out vec3 WorldPos;
layout(location = 1) out vec3 Normal;

void main() {
    gl_Position = ubo.proj * ubo.view * ubo.model * vec4(inPosition, 1.0);
    WorldPos = vec4(ubo.model * vec4(inPosition, 1.0)).xyz; // TODO vec4 required?
    Normal = vec4(ubo.model * vec4(inNormal, 1.0)).xyz;
}