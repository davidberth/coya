#include "buffer.h"

#include "device.h"
#include "command_buffer.h"
#include "util.h"

#include "core/logger.h"
#include "core/memory.h"
#include <string.h>

extern VulkanContext vulkan_context;

bool vulkan_buffer_create(unsigned int size, VkBufferUsageFlagBits usage,
  unsigned int memory_property_flags, bool bind_on_create,
  VulkanBuffer *out_buffer) {

    memset(out_buffer, 0, sizeof(VulkanBuffer));
    out_buffer->total_size = size;
    out_buffer->usage = usage;
    out_buffer->memory_property_flags = memory_property_flags;

    VkBufferCreateInfo buffer_info = {VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO};
    buffer_info.size = size;
    buffer_info.usage = usage;
    buffer_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    buffer_info.flags = 0;
    buffer_info.pNext = nullptr;

    vk_check(vkCreateBuffer(vulkan_context.device.logical_device, &buffer_info,
      vulkan_context.allocator, &out_buffer->handle));

    VkMemoryRequirements requirements;
    vkGetBufferMemoryRequirements(
      vulkan_context.device.logical_device, out_buffer->handle, &requirements);
    out_buffer->memory_index = vulkan_context.find_memory_index(
      requirements.memoryTypeBits, memory_property_flags);

    if (out_buffer->memory_index < 0) {
        elog("failed to find memory index");
        return false;
    }

    VkMemoryAllocateInfo alloc_info = {VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO};
    alloc_info.allocationSize = requirements.size;
    alloc_info.memoryTypeIndex = (unsigned int)out_buffer->memory_index;

    VkResult result = vkAllocateMemory(vulkan_context.device.logical_device,
      &alloc_info, vulkan_context.allocator, &out_buffer->memory);

    if (result != VK_SUCCESS) {
        elog("failed to allocate memory");
        return false;
    }

    if (bind_on_create) {
        vulkan_buffer_bind(out_buffer, 0);
    }

    return true;
}

void vulkan_buffer_destroy(VulkanBuffer *buffer) {
    if (buffer->memory) {
        vkFreeMemory(vulkan_context.device.logical_device, buffer->memory,
          vulkan_context.allocator);
        buffer->memory = VK_NULL_HANDLE;
    }
    if (buffer->handle) {
        vkDestroyBuffer(vulkan_context.device.logical_device, buffer->handle,
          vulkan_context.allocator);
        buffer->handle = VK_NULL_HANDLE;
    }
    buffer->total_size = 0;
    buffer->usage = 0;
    buffer->is_locked = false;
}

bool vulkan_buffer_resize(unsigned int new_size, VulkanBuffer *buffer,
  VkQueue queue, VkCommandPool pool) {
    VkBufferCreateInfo buffer_info = {VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO};
    buffer_info.size = new_size;
    buffer_info.usage = buffer->usage;
    buffer_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    VkBuffer new_buffer;
    vk_check(vkCreateBuffer(vulkan_context.device.logical_device, &buffer_info,
      vulkan_context.allocator, &new_buffer));

    VkMemoryRequirements requirements;
    vkGetBufferMemoryRequirements(
      vulkan_context.device.logical_device, new_buffer, &requirements);

    VkMemoryAllocateInfo alloc_info = {VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO};
    alloc_info.allocationSize = requirements.size;
    alloc_info.memoryTypeIndex = (unsigned int)buffer->memory_index;

    VkDeviceMemory new_memory;
    VkResult result = vkAllocateMemory(vulkan_context.device.logical_device,
      &alloc_info, vulkan_context.allocator, &new_memory);
    if (result != VK_SUCCESS) {
        elog("failed to allocate memory");
        return false;
    }

    vk_check(vkBindBufferMemory(
      vulkan_context.device.logical_device, new_buffer, new_memory, 0));

    vulkan_buffer_copy_to(
      pool, 0, queue, buffer->handle, 0, new_buffer, 0, buffer->total_size);

    vkDeviceWaitIdle(vulkan_context.device.logical_device);

    if (buffer->memory) {
        vkFreeMemory(vulkan_context.device.logical_device, buffer->memory,
          vulkan_context.allocator);
        buffer->memory = 0;
    }

    if (buffer->handle) {
        vkDestroyBuffer(vulkan_context.device.logical_device, buffer->handle,
          vulkan_context.allocator);
        buffer->handle = 0;
    }

    buffer->total_size = new_size;
    buffer->memory = new_memory;
    buffer->handle = new_buffer;

    return true;
}

void vulkan_buffer_bind(VulkanBuffer *buffer, unsigned int offset) {
    vk_check(vkBindBufferMemory(vulkan_context.device.logical_device,
      buffer->handle, buffer->memory, offset));
}

void *vulkan_buffer_lock_memory(VulkanBuffer *buffer, unsigned int offset,
  unsigned int size, unsigned int flags) {
    void *data;
    vk_check(vkMapMemory(vulkan_context.device.logical_device, buffer->memory,
      offset, size, flags, &data));
    buffer->is_locked = true;
    return data;
}
void vulkan_buffer_unlock_memory(VulkanBuffer *buffer) {
    vkUnmapMemory(vulkan_context.device.logical_device, buffer->memory);
    buffer->is_locked = false;
}

void vulkan_buffer_load_data(VulkanBuffer *buffer, unsigned int offset,
  unsigned int size, unsigned int flags, const void *data) {
    void *data_ptr;
    vk_check(vkMapMemory(vulkan_context.device.logical_device, buffer->memory,
      offset, size, flags, &data_ptr));
    memcpy(data_ptr, data, size);
    vkUnmapMemory(vulkan_context.device.logical_device, buffer->memory);
}

void vulkan_buffer_copy_to(VkCommandPool pool, VkFence fence, VkQueue queue,
  VkBuffer source, unsigned int source_offset, VkBuffer dest,
  unsigned int dest_offset, unsigned int size) {

    vkQueueWaitIdle(queue);
    VulkanCommandBuffer temp_command_buffer;
    vulkan_command_buffer_allocate_and_begin_single_use(
      pool, &temp_command_buffer);

    VkBufferCopy copy_region = {};
    copy_region.srcOffset = source_offset;
    copy_region.dstOffset = dest_offset;
    copy_region.size = size;

    vkCmdCopyBuffer(temp_command_buffer.handle, source, dest, 1, &copy_region);

    // submit the command buffer and wait for it to complete
    vulkan_command_buffer_end_single_use(pool, &temp_command_buffer, queue);
}