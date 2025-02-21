#pragma once

#include "renderer/vulkan/types.h"

void vulkan_renderpass_create(VulkanRenderpass *out_renderpass, float x,
  float y, float w, float h, float r, float g, float b, float a, float depth,
  unsigned int stencil);

void vulkan_renderpass_destroy(VulkanRenderpass *renderpass);

void vulkan_renderpass_begin(VulkanCommandBuffer *command_buffer,
  VulkanRenderpass *renderpass, VkFramebuffer frame_buffer);

void vulkan_renderpass_end(
  VulkanCommandBuffer *command_buffer, VulkanRenderpass *renderpass);
