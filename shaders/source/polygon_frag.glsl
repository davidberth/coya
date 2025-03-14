#version 450 core
# extension GL_ARB_separate_shader_objects: enable

layout(location = 0) out vec4 out_color;

layout(set = 1, binding = 0) uniform local_uniform_object {
    vec4 diffuse_color;
} object_ubo;

void main() {
    out_color = object_ubo.diffuse_color;   
}
