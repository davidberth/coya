#include "shader.h"
#include "core/logger.h"
#include "renderer/vulkan/types.h"
#include "renderer/vulkan/shader_utils.h"
#include "renderer/vulkan/pipeline.h"
#include <vulkan/vulkan_core.h>
#include "math/types.h"
#include <string.h>
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
    viewport.y = (float)(vulkan_context.framebuffer_height);
    viewport.width = (float)(vulkan_context.framebuffer_width);
    viewport.height = -(float)(vulkan_context.framebuffer_height);
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;

    // scissor
    VkRect2D scissor;
    scissor.offset.x = scissor.offset.y = 0;
    scissor.extent.width = vulkan_context.framebuffer_width;
    scissor.extent.height = vulkan_context.framebuffer_height;

    // attributes
    unsigned int offset = 0;
    constexpr int attribute_count = 1;
    VkVertexInputAttributeDescription attribute_descriptions[attribute_count];
    VkFormat formats[attribute_count] = {VK_FORMAT_R32G32B32_SFLOAT};
    unsigned int sizes[attribute_count] = {sizeof(vec3)};
    for (unsigned int i = 0; i < attribute_count; ++i) {
        attribute_descriptions[i].binding = 0;
        attribute_descriptions[i].location = i;
        attribute_descriptions[i].format = formats[i];
        attribute_descriptions[i].offset = offset;
        offset += sizes[i];
    }

    VkPipelineShaderStageCreateInfo
      stage_create_infos[object_shader_stage_count];
    memset(stage_create_infos, 0, sizeof(stage_create_infos));
    for (unsigned int i = 0; i < object_shader_stage_count; ++i) {
        stage_create_infos[i].sType =
          VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        stage_create_infos[i].sType = out_shader->stages[i].stage_info.sType;
        stage_create_infos[i] = out_shader->stages[i].stage_info;
    }

    if (!vulkan_graphics_pipeline_create(&vulkan_context.main_renderpass,
          attribute_count, attribute_descriptions, 0, 0,
          object_shader_stage_count, stage_create_infos, viewport, scissor,
          false, &out_shader->pipeline)) {
        elog("Unable to create graphics pipeline");
        return false;
    }

    return true;
}

void shader_destroy(VulkanShader *shader) {
    // log that we're destroying shader modules
    vulkan_pipeline_destroy(&shader->pipeline);

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

void shader_use(VulkanShader *shader) {
    unsigned int image_index = vulkan_context.image_index;
    vulkan_pipeline_bind(&vulkan_context.graphics_command_buffers[image_index],
      VK_PIPELINE_BIND_POINT_GRAPHICS, &shader->pipeline);
}