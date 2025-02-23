#pragma once

#include "types.h"

void vulkan_framebuffer_create(VulkanRenderpass *renderpass, unsigned int width,
  unsigned int height, unsigned int attachment_count, VkImageView *attachments,
  VulkanFramebuffer *out_framebuffer);

void vulkan_framebuffer_destroy(VulkanFramebuffer *framebuffer);