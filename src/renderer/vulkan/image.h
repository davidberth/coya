#pragma once

#include "renderer/vulkan/types.h"

void vulkan_image_create(VkImageType image_type, unsigned int width,
                         unsigned int height, VkFormat format,
                         VkImageTiling tiling, VkImageUsageFlags usage,
                         VkMemoryPropertyFlags memory_flags, bool create_view,
                         VkImageAspectFlags view_aspect_flags,
                         VulkanImage *out_image);

void vulkan_image_view_create(VkFormat format, VulkanImage *image,
                              VkImageAspectFlags aspect_flags);

void vulkan_image_destroy(VulkanImage *image);
