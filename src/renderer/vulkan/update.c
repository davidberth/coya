#include "renderer/renderer.h"
#include "core/event.h"
#include "renderer/vulkan/types.h"
#include "core/logger.h"
#include "math/omath.h"

extern VulkanContext vulkan_context;
extern RendererGlobalState renderer_state;

void renderer_on_resize(EventContext event_context) {
    vulkan_context.framebuffer_width = event_context.uint[0];
    vulkan_context.framebuffer_height = event_context.uint[1];
    vulkan_context.framebuffer_resize_needed = true;

    // TODO: graphics API agnostic code should be moved to exposed generic
    // render routines
    float aspect_ratio = (float)vulkan_context.framebuffer_width /
                         (float)vulkan_context.framebuffer_height;
    renderer_state.projection = mat4_perspective(O_DEG2RAD * 45.0f,
      aspect_ratio, renderer_state.near_clip, renderer_state.far_clip);

    ilog("renderer resize requested: %d, %d", vulkan_context.framebuffer_width,
      vulkan_context.framebuffer_height);
}
