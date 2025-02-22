#include "command_buffer.h"
#include "types.h"
#include "util.h"

extern VulkanContext vulkan_context;

void vulkan_command_buffer_allocate(VkCommandPool pool, bool is_primary,
  VulkanCommandBuffer *out_command_buffer) {

    // TODO: zero out command buffer memory
    VkCommandBufferAllocateInfo allocate_info = {
      VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO};
    allocate_info.commandPool = pool;
    allocate_info.level = is_primary ? VK_COMMAND_BUFFER_LEVEL_PRIMARY
                                     : VK_COMMAND_BUFFER_LEVEL_SECONDARY;
    allocate_info.commandBufferCount = 1;

    out_command_buffer->state = COMMAND_BUFFER_STATE_NOT_ALLOCATED;
    vk_check(vkAllocateCommandBuffers(vulkan_context.device.logical_device,
      &allocate_info, &out_command_buffer->handle));
    out_command_buffer->state = COMMAND_BUFFER_STATE_READY;
}

void vulkan_command_buffer_free(
  VkCommandPool pool, VulkanCommandBuffer *command_buffer) {
    vkFreeCommandBuffers(
      vulkan_context.device.logical_device, pool, 1, &command_buffer->handle);

    command_buffer->handle = 0;
    command_buffer->state = COMMAND_BUFFER_STATE_NOT_ALLOCATED;
}

void vulkan_command_buffer_begin(VulkanCommandBuffer *command_buffer,
  bool is_single_use, bool is_renderpass_continue, bool is_simulataneous_use) {
    VkCommandBufferBeginInfo begin_info = {
      VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO};
    if (is_single_use) {
        begin_info.flags |= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
    }
    if (is_renderpass_continue) {
        begin_info.flags |= VK_COMMAND_BUFFER_USAGE_RENDER_PASS_CONTINUE_BIT;
    }
    if (is_simulataneous_use) {
        begin_info.flags |= VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;
    }

    vk_check(vkBeginCommandBuffer(command_buffer->handle, &begin_info));
    command_buffer->state = COMMAND_BUFFER_STATE_RECORDING;
}

void vulkan_command_buffer_end(VulkanCommandBuffer *command_buffer) {
    vk_check(vkEndCommandBuffer(command_buffer->handle));
    command_buffer->state = COMMAND_BUFFER_STATE_RECORDING_ENDED;
}

void vulkan_command_buffer_update_submitted(
  VulkanCommandBuffer *command_buffer) {
    command_buffer->state = COMMAND_BUFFER_STATE_SUBMITTED;
}

void vulkan_command_buffer_reset(VulkanCommandBuffer *command_buffer) {
    command_buffer->state = COMMAND_BUFFER_STATE_READY;
}

void vulkan_command_buffer_allocate_and_begin_single_use(
  VkCommandPool pool, VulkanCommandBuffer *out_command_buffer) {
    vulkan_command_buffer_allocate(pool, true, out_command_buffer);
    vulkan_command_buffer_begin(out_command_buffer, true, false, false);
}

void vulkan_command_buffer_end_single_use(
  VkCommandPool pool, VulkanCommandBuffer *command_buffer, VkQueue queue) {

    vulkan_command_buffer_end(command_buffer);
    VkSubmitInfo submit_info = {VK_STRUCTURE_TYPE_SUBMIT_INFO};
    submit_info.commandBufferCount = 1;
    submit_info.pCommandBuffers = &command_buffer->handle;
    vk_check(vkQueueSubmit(queue, 1, &submit_info, VK_NULL_HANDLE));
    vkQueueWaitIdle(queue);
    vulkan_command_buffer_free(pool, command_buffer);
}
