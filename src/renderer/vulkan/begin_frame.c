#include "renderer/renderer.h"
#include "types.h"
#include "core/logger.h"
#include "device.h"
#include "swapchain.h"
#include "fence.h"
#include "command_buffer.h"
#include "renderpass.h"
#include "framebuffer.h"
#include "util.h"

extern VulkanContext vulkan_context;

bool recreate_swapchain() {
    if (vulkan_context.recreating_swapchain) {
        dlog("already recreating swapchain");
        return false;
    }
    if (vulkan_context.framebuffer_width == 0 ||
        vulkan_context.framebuffer_height == 0) {
        dlog("framebuffer width or height is 0.");
        return false;
    }

    vulkan_context.recreating_swapchain = true;

    for (unsigned int i = 0; i < vulkan_context.swapchain.image_count; ++i) {
        vulkan_context.images_in_flight[i] = VK_NULL_HANDLE;
    }

    // TODO: remove these? (only happens on resize)
    vulkan_device_query_swapchain_support();
    vulkan_device_detect_depth_format();

    vulkan_swapchain_recreate(vulkan_context.framebuffer_width,
      vulkan_context.framebuffer_height, &vulkan_context.swapchain);

    // cleanup the swapchain
    for (unsigned int i = 0; i < vulkan_context.swapchain.image_count; ++i) {
        vulkan_command_buffer_free(vulkan_context.device.graphics_command_pool,
          &vulkan_context.graphics_command_buffers[i]);
    }

    for (unsigned int i = 0; i < vulkan_context.swapchain.image_count; ++i) {
        vulkan_framebuffer_destroy(&vulkan_context.swapchain.framebuffers[i]);
    }

    vulkan_context.main_renderpass.x = 0;
    vulkan_context.main_renderpass.y = 0;
    vulkan_context.main_renderpass.w = vulkan_context.framebuffer_width;
    vulkan_context.main_renderpass.h = vulkan_context.framebuffer_height;

    vulkan_framebuffer_create(vulkan_context.main_renderpass,
      vulkan_context.framebuffer_width, vulkan_context.framebuffer_height,
      vulkan_context.swapchain.frame_buffers, attachment_count, attachments,
      &vulkan_context.swapchain.framebuffers[i]);

    create_command_buffers(&vulkan_context.swapchain);

    vulkan_context.recreating_swapchain = false;
}

bool renderer_begin_frame(float delta_time) {
    VulkanDevice *device = &vulkan_context.device;

    if (vulkan_context.recreating_swapchain) {
        VkResult result = vkDeviceWaitIdle(device->logical_device);
        if (!vulkan_result_is_success(result)) {
            flog("failed to wait for device idle");
            return false;
        }
        ilog("recreating swapchain, booting");
        return false;
    }

    // check if the framebuffer has been resized
    if (vulkan_context.framebuffer_resize_needed) {
        ilog("framebuffer resize needed");
        vulkan_context.framebuffer_resize_needed = false;
        VkResult result = vkDeviceWaitIdle(device->logical_device);
        if (!vulkan_result_is_success(result)) {
            flog("failed to wait for device idle");
            return false;
        }

        if (!recreate_swapchain()) {
            return false;
        }

        ilog("resized. booting");
        return false;
    }

    if (!vulkan_fence_wait(
          &vulkan_context.in_flight_fences[vulkan_context.current_frame],
          UINT64_MAX)) {
        flog("failed to wait for in flight fence");
        return false;
    }

    if (!vulkan_swapchain_acquire_next_image_index(&vulkan_context.swapchain,
          UINT64_MAX,
          vulkan_context
            .image_available_semaphores[vulkan_context.current_frame],
          0, &vulkan_context.image_index)) {
        return false;
    }

    VulkanCommandBuffer *command_buffer =
      &vulkan_context.graphics_command_buffers[vulkan_context.image_index];

    vulkan_command_buffer_reset(command_buffer);
    vulkan_command_buffer_begin(command_buffer, false, false, false);

    VkViewport viewport;
    viewport.x = 0.0f;
    viewport.y = (float)vulkan_context.framebuffer_height;
    viewport.width = (float)vulkan_context.framebuffer_width;
    viewport.height = -(float)vulkan_context.framebuffer_height;
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;

    VkRect2D scissor;
    scissor.offset.x = scissor.offset.y = 0;
    scissor.extent.width = vulkan_context.framebuffer_width;
    scissor.extent.height = vulkan_context.framebuffer_height;

    vkCmdSetViewport(command_buffer->handle, 0, 1, &viewport);
    vkCmdSetScissor(command_buffer->handle, 0, 1, &scissor);

    vulkan_context.main_renderpass.w = vulkan_context.framebuffer_width;
    vulkan_context.main_renderpass.h = vulkan_context.framebuffer_height;

    vulkan_renderpass_begin(command_buffer, &vulkan_context.main_renderpass,
      vulkan_context.swapchain.framebuffers[vulkan_context.image_index].handle);

    return true;
}
