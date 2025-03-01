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

    // pipeline creation
    VkViewport viewport;
    viewport.x = 0.0f;
    viewport.y = (float)(vulkan_context.swapchain.extent.height);
    viewport.width = (float)(vulkan_context.swapchain.extent.width);
    viewport.height = -(float)(vulkan_context.swapchain.extent.height);
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;

    // scissor
    VkRect2D scissor;
    scissor.offset.x = scissor.offset.y = 0;
    scissor.extent.width = vulkan_context.framebuffer_width;
    scissor.extent.height = vulkan_context.framebuffer_height;

    // attributes

    return true;
}

void shader_destroy(VulkanShader *shader) {
    // log that we're destroying shader modules
    dlog("destroying shader modules");

    for (unsigned int i = 0; i < object_shader_stage_count; ++i) {
        if (shader->stages[i].shader_handle != VK_NULL_HANDLE) {
            vkDestroyShaderModule(vulkan_context.device.logical_device,
              shader->stages[i].shader_handle, vulkan_context.allocator);
            shader->stages[i].shader_handle = VK_NULL_HANDLE;

            // make sure to clear the stage_info module reference as well
            shader->stages[i].stage_info.module = VK_NULL_HANDLE;
        }
    }
}

void shader_use(VulkanShader *shader) {}