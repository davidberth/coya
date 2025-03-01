#include "shader_utils.h"
#include <stdio.h>
#include <string.h>
#include "core/logger.h"
#include "core/memory.h"
#include "platform/filesystem.h"
#include "renderer/vulkan/util.h"

extern VulkanContext vulkan_context;

bool create_shader_module(const char *name, const char *type_str,
  VkShaderStageFlagBits shader_stage_flag, unsigned int stage_index,
  VulkanShaderStage *shader_stages) {
    // TODO: maybe make this dynamic?
    char file_name[256];
    snprintf(file_name, sizeof(file_name), "shaders/compiled/%s_%s.spv", name,
      type_str);

    memset(&shader_stages[stage_index].create_info, 0,
      sizeof(VkShaderModuleCreateInfo));

    shader_stages[stage_index].create_info.sType =
      VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;

    FileHandle handle;
    if (!filesystem_open(file_name, FILE_MODE_READ, true, &handle)) {
        elog("Failed to open shader file '%s'", file_name);
        return false;
    }

    unsigned int size = 0;
    unsigned int *file_buffer = 0;
    if (!filesystem_read_all_bytes(&handle, &file_buffer, &size)) {
        elog("Failed to read shader file '%s'", file_name);
        return false;
    }
    shader_stages[stage_index].create_info.codeSize = size;
    shader_stages[stage_index].create_info.pCode = (unsigned int *)file_buffer;

    filesystem_close(&handle);

    vk_check(vkCreateShaderModule(vulkan_context.device.logical_device,
      &shader_stages[stage_index].create_info, vulkan_context.allocator,
      &shader_stages[stage_index].shader_handle));

    memset(&shader_stages[stage_index].stage_info, 0,
      sizeof(VkPipelineShaderStageCreateInfo));
    shader_stages[stage_index].stage_info.sType =
      VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    shader_stages[stage_index].stage_info.stage = shader_stage_flag;
    shader_stages[stage_index].stage_info.module =
      shader_stages[stage_index].shader_handle;
    shader_stages[stage_index].stage_info.pName = "main";

    if (file_buffer) {
        ofree(file_buffer);
        file_buffer = 0;
    }

    return true;
}
