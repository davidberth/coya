#include "renderer/renderer.h"
#include "core/event.h"
#include "renderer/vulkan/types.h"
#include "core/logger.h"

extern VulkanContext vulkan_context;

void renderer_on_resize(EventContext event_context) {
    vulkan_context.framebuffer_width = event_context.uint[0];
    vulkan_context.framebuffer_height = event_context.uint[1];
    vulkan_context.framebuffer_resize_needed = true;

    ilog("renderer resize requested: %d, %d", vulkan_context.framebuffer_width,
      vulkan_context.framebuffer_height);
}