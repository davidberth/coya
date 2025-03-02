#pragma once

#include "types.h"

extern VulkanContext vulkan_context;

bool vulkan_buffer_create(unsigned int size, VkBufferUsageFlagBits usage,
  unsigned int memory_property_flags, bool bind_on_create,
  VulkanBuffer *out_buffer);

void vulkan_buffer_destroy(VulkanBuffer *buffer);

bool vulkan_buffer_resize(unsigned int new_size, VulkanBuffer *buffer,
  VkQueue queue, VkCommandPool pool);

void vulkan_buffer_bind(VulkanBuffer *buffer, unsigned int offset);

void *vulkan_buffer_lock_memory(VulkanBuffer *buffer, unsigned int offset,
  unsigned int size, unsigned int flags);
void vulkan_buffer_unlock_memory(VulkanBuffer *buffer);

void vulkan_buffer_load_data(VulkanBuffer *buffer, unsigned int offset,
  unsigned int size, unsigned int flags, const void *data);

void vulkan_buffer_copy_to(VkCommandPool pool, VkFence fence, VkQueue queue,
  VkBuffer source, unsigned int source_offset, VkBuffer dest,
  unsigned int dest_offset, unsigned int size);
