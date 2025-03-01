#pragma once

#include "types.h"

bool vulkan_graphics_pipeline_create(VulkanRenderpass *renderpass,
  unsigned int attribute_count, VkVertexInputAttributeDescription *attributes,
  unsigned int descriptor_set_layout_count,
  VkDescriptorSetLayout *descriptor_set_layouts, unsigned int stage_count,
  VkPipelineShaderStageCreateInfo *stages, VkViewport viewport,
  VkRect2D scissor, bool is_wireframe, VulkanPipeline *out_pipeline);

void vulkan_pipeline_destroy(VulkanPipeline *pipeline);

void vulkan_pipeline_bind(VulkanCommandBuffer *command_buffer,
  VkPipelineBindPoint bind_point, VulkanPipeline *pipeline);
