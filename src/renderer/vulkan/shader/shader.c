#include "shader.h"
#include "core/logger.h"
#include "renderer/vulkan/types.h"
#include "renderer/vulkan/shader_utils.h"

#define BUILTIN_SHADER_NAME_OBJECT "polygon"

extern VulkanContext vulkan_context;

bool shader_create(VulkanShader *out_shader) {
    char stage_type_strs[object_shader_stage_count][5] = {"vert", "frag"};
    VkShaderStageFlagBits stage_types[object_shader_stage_count] = {
      VK_SHADER_STAGE_VERTEX_BIT, VK_SHADER_STAGE_FRAGMENT_BIT};

    for (unsigned int i = 0; i < object_shader_stage_count; ++i) {
        if (!create_shader_module(BUILTIN_SHADER_NAME_OBJECT,
              stage_type_strs[i], stage_types[i], i, out_shader->stages)) {
            elog("Unable to create %s shader module for '%s'.",
              stage_type_strs[i], BUILTIN_SHADER_NAME_OBJECT);
            return false;
        }
    }

    ilog("successfully created shader modules for '%s'",
      BUILTIN_SHADER_NAME_OBJECT);

    // TODO: descriptors
    return true;
}

void shader_destroy(VulkanShader *shader) {}

void shader_use(VulkanShader *shader) {}