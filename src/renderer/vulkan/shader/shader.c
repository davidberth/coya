#include "shader.h"
#include "renderer/vulkan/types.h"

extern VulkanContext vulkan_context;

bool shader_create(VulkanShader *out_shader) {
    char stage_type_strs[object_shader_stage_count][5] = {"vert", "frag"};
    VkShaderStageFlagBits stage_types[object_shader_stage_count] = {
      VK_SHADER_STAGE_VERTEX_BIT, VK_SHADER_STAGE_FRAGMENT_BIT};

    return true;
}

void shader_destroy(VulkanShader *shader) {}

void shader_use(VulkanShader *shader) {}
