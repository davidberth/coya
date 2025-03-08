#include "shader.h"
#include "core/logger.h"
#include "renderer/vulkan/types.h"
#include "renderer/vulkan/shader_utils.h"
#include "renderer/vulkan/pipeline.h"
#include <vulkan/vulkan.h>
#include "math/types.h"
#include <string.h>
#include "renderer/vulkan/util.h"
#include "renderer/vulkan/buffer.h"

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

    // global descriptors
    VkDescriptorSetLayoutBinding global_ubo_layout_binding;
    global_ubo_layout_binding.binding = 0;
    global_ubo_layout_binding.descriptorType =
      VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    global_ubo_layout_binding.descriptorCount = 1;
    global_ubo_layout_binding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
    global_ubo_layout_binding.pImmutableSamplers = nullptr;

    VkDescriptorSetLayoutCreateInfo global_layout_info = {
      VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO};
    global_layout_info.bindingCount = 1;
    global_layout_info.pBindings = &global_ubo_layout_binding;
    vk_check(vkCreateDescriptorSetLayout(vulkan_context.device.logical_device,
      &global_layout_info, vulkan_context.allocator,
      &out_shader->global_descriptor_set_layout));

    // global descriptor pool: used for global items such as the view and
    // projection matrix
    VkDescriptorPoolSize global_pool_size;
    global_pool_size.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    global_pool_size.descriptorCount = vulkan_context.swapchain.image_count;

    VkDescriptorPoolCreateInfo global_pool_info = {
      VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO};
    global_pool_info.poolSizeCount = 1;
    global_pool_info.pPoolSizes = &global_pool_size;
    global_pool_info.maxSets = vulkan_context.swapchain.image_count;
    vk_check(vkCreateDescriptorPool(vulkan_context.device.logical_device,
      &global_pool_info, vulkan_context.allocator,
      &out_shader->global_descriptor_pool));

    // pipeline creation
    VkViewport viewport;
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = (float)(vulkan_context.framebuffer_width);
    viewport.height = (float)(vulkan_context.framebuffer_height);
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

    // descriptor set layouts
    constexpr int descriptor_set_layout_count = 1;
    VkDescriptorSetLayout layouts[descriptor_set_layout_count] = {
      out_shader->global_descriptor_set_layout};

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
          attribute_count, attribute_descriptions, descriptor_set_layout_count,
          layouts, object_shader_stage_count, stage_create_infos, viewport,
          scissor, false, &out_shader->pipeline)) {
        elog("Unable to create graphics pipeline");
        return false;
    }

    if (!vulkan_buffer_create(sizeof(GlobalUniformObject) * 3,
          VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
          VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT |
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
            VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
          true, &out_shader->global_uniform_buffer)) {
        elog("Unable to create global uniform buffer");
        return false;
    }

    VkDescriptorSetLayout global_layouts[3] = {
      out_shader->global_descriptor_set_layout,
      out_shader->global_descriptor_set_layout,
      out_shader->global_descriptor_set_layout};

    VkDescriptorSetAllocateInfo alloc_info = {
      VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO};
    alloc_info.descriptorPool = out_shader->global_descriptor_pool;
    alloc_info.descriptorSetCount = 3;
    alloc_info.pSetLayouts = global_layouts;

    vk_check(vkAllocateDescriptorSets(vulkan_context.device.logical_device,
      &alloc_info, out_shader->global_descriptor_sets));

    ilog("successfully created shader modules for '%s'",
      BUILTIN_SHADER_NAME_OBJECT);

    return true;
}

void shader_destroy(VulkanShader *shader) {
    VkDevice logical_device = vulkan_context.device.logical_device;
    vulkan_buffer_destroy(&shader->global_uniform_buffer);

    vulkan_pipeline_destroy(&shader->pipeline);

    vkDestroyDescriptorPool(
      logical_device, shader->global_descriptor_pool, vulkan_context.allocator);

    vkDestroyDescriptorSetLayout(logical_device,
      shader->global_descriptor_set_layout, vulkan_context.allocator);

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

void vulkan_object_shader_update_global_state(VulkanShader *shader) {
    unsigned int image_index = vulkan_context.image_index;
    VkCommandBuffer command_buffer =
      vulkan_context.graphics_command_buffers[image_index].handle;
    VkDescriptorSet global_descriptor =
      shader->global_descriptor_sets[image_index];

    unsigned int range = sizeof(GlobalUniformObject);
    unsigned int offset = range * image_index;

    vulkan_buffer_load_data(
      &shader->global_uniform_buffer, offset, range, 0, &shader->global_ubo);

    VkDescriptorBufferInfo buffer_info;
    buffer_info.buffer = shader->global_uniform_buffer.handle;
    buffer_info.offset = offset;
    buffer_info.range = range;

    VkWriteDescriptorSet descriptor_write = {
      VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET};
    descriptor_write.dstSet = shader->global_descriptor_sets[image_index];
    descriptor_write.dstBinding = 0;
    descriptor_write.dstArrayElement = 0;
    descriptor_write.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    descriptor_write.descriptorCount = 1;
    descriptor_write.pBufferInfo = &buffer_info;

    vkUpdateDescriptorSets(
      vulkan_context.device.logical_device, 1, &descriptor_write, 0, nullptr);

    vkCmdBindDescriptorSets(command_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
      shader->pipeline.layout, 0, 1, &global_descriptor, 0, nullptr);
}