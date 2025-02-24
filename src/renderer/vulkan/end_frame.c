#include "renderer/renderer.h"
#include "types.h"
#include "renderpass.h"
#include "command_buffer.h"
#include "fence.h"
#include "swapchain.h"
#include <vulkan/vulkan_core.h>
#include "core/logger.h"

extern VulkanContext vulkan_context;

bool renderer_end_frame(float delta_time) {
    VulkanCommandBuffer *command_buffer =
      &vulkan_context.graphics_command_buffers[vulkan_context.image_index];

    vulkan_renderpass_end(command_buffer, &vulkan_context.main_renderpass);

    vulkan_command_buffer_end(command_buffer);

    if (vulkan_context.images_in_flight[vulkan_context.image_index] !=
        VK_NULL_HANDLE) {
        vulkan_fence_wait(
          vulkan_context.images_in_flight[vulkan_context.image_index],
          UINT64_MAX);
    }

    vulkan_context.images_in_flight[vulkan_context.image_index] =
      &vulkan_context.in_flight_fences[vulkan_context.current_frame];

    vulkan_fence_reset(
      &vulkan_context.in_flight_fences[vulkan_context.current_frame]);

    VkSubmitInfo submit_info = {VK_STRUCTURE_TYPE_SUBMIT_INFO};
    submit_info.commandBufferCount = 1;
    submit_info.pCommandBuffers = &command_buffer->handle;
    submit_info.signalSemaphoreCount = 1;
    submit_info.pSignalSemaphores =
      &vulkan_context.queue_complete_semaphores[vulkan_context.current_frame];
    submit_info.waitSemaphoreCount = 1;
    submit_info.pWaitSemaphores =
      &vulkan_context.image_available_semaphores[vulkan_context.current_frame];

    VkPipelineStageFlags wait_stages[1] = {
      VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};

    submit_info.pWaitDstStageMask = wait_stages;

    VkResult result =
      vkQueueSubmit(vulkan_context.device.graphics_queue, 1, &submit_info,
        vulkan_context.in_flight_fences[vulkan_context.current_frame].handle);

    if (result != VK_SUCCESS) {
        flog("vkQueueSubmit failed");
        return false;
    }

    vulkan_command_buffer_update_submitted(command_buffer);

    vulkan_swapchain_present(&vulkan_context.swapchain,
      vulkan_context.device.graphics_queue, vulkan_context.device.present_queue,
      vulkan_context.queue_complete_semaphores[vulkan_context.current_frame],
      vulkan_context.image_index);

    return true;
}