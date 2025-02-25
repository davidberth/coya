#version 450 core

layout(set = 1, binding = 0, std140) uniform UniformBufferObject {
    mat4 model_view_proj;
} ubo;

layout(location = 0) in vec2 a_pos;
layout(location = 1) in vec3 a_color;

layout(location = 0) out vec3 color;

void main()
{
    gl_Position = ubo.model_view_proj * vec4(a_pos, 0.0, 1.0);
    color = a_color;
}