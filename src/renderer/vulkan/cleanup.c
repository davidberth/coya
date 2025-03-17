#include "core/logger.h"
#include "core/memory.h"
#include "types.h"
#include "device.h"
#include "swapchain.h"
#include "renderpass.h"
#include "command_buffer.h"
#include "framebuffer.h"
#include "fence.h"
#include "shader/shader.h"
#include "buffer.h"
#include <vulkan/vulkan_core.h>
#include "renderer/renderer.h"

extern VulkanContext vulkan_context;
extern RendererGlobalState renderer_state;

void renderer_cleanup() {
    vkDeviceWaitIdle(vulkan_context.device.logical_device);

    vulkan_buffer_destroy(&vulkan_context.main_vertex_buffer);
    vulkan_buffer_destroy(&vulkan_context.main_index_buffer);

    ilog("destroying default texture");
    renderer_destroy_texture(&renderer_state.default_texture);

    ilog("destroying test diffuse texture");
    renderer_destroy_texture(&renderer_state.test_diffuse);

    shader_destroy(&vulkan_context.main_shader);
#ifdef _DEBUG
    dlog("cleaning up the vulkan debugger");
    if (vulkan_context.debug_messenger) {

        PFN_vkDestroyDebugUtilsMessengerEXT func =
          (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
            vulkan_context.instance, "vkDestroyDebugUtilsMessengerEXT");

        func(vulkan_context.instance, vulkan_context.debug_messenger,
          vulkan_context.allocator);
    }
#endif

    for (unsigned i = 0; i < vulkan_context.swapchain.max_frames_in_flight;
      ++i) {
        if (vulkan_context.image_available_semaphores[i]) {
            vkDestroySemaphore(vulkan_context.device.logical_device,
              vulkan_context.image_available_semaphores[i],
              vulkan_context.allocator);
            vulkan_context.image_available_semaphores[i] = nullptr;
        }

        if (vulkan_context.queue_complete_semaphores[i]) {
            vkDestroySemaphore(vulkan_context.device.logical_device,
              vulkan_context.queue_complete_semaphores[i],
              vulkan_context.allocator);
            vulkan_context.queue_complete_semaphores[i] = nullptr;
        }
        vulkan_fence_destroy(&vulkan_context.in_flight_fences[i]);
    }

    ilog("cleaning up fences and semaphores");
    ofree(vulkan_context.image_available_semaphores);
    ofree(vulkan_context.queue_complete_semaphores);
    ofree(vulkan_context.in_flight_fences);
    ofree(vulkan_context.images_in_flight);

    ilog("cleaning up swapchain framebuffers");

    for (unsigned int i = 0; i < vulkan_context.swapchain.image_count; i++) {
        if (vulkan_context.swapchain.framebuffers[i].handle) {
            vulkan_framebuffer_destroy(
              &vulkan_context.swapchain.framebuffers[i]);
        }
    }
    ofree(vulkan_context.swapchain.framebuffers);

    ilog("cleaning up graphics command buffers");
    for (unsigned int i = 0; i < vulkan_context.swapchain.image_count; i++) {
        if (vulkan_context.graphics_command_buffers[i].handle) {
            vulkan_command_buffer_free(
              vulkan_context.device.graphics_command_pool,
              &vulkan_context.graphics_command_buffers[i]);
        }
    }
    ofree(vulkan_context.graphics_command_buffers);

    ilog("cleaning up the command pool");
    vkDestroyCommandPool(vulkan_context.device.logical_device,
      vulkan_context.device.graphics_command_pool, vulkan_context.allocator);

    ilog("cleaning up the main renderpass");
    vulkan_renderpass_destroy(&vulkan_context.main_renderpass);

    ilog("cleaning up the swapchain");
    vulkan_swapchain_destroy(&vulkan_context.swapchain);

    ilog("cleaning up device");
    vulkan_device_cleanup();

    ilog("cleaning up the surface");
    if (vulkan_context.surface) {
        vkDestroySurfaceKHR(vulkan_context.instance, vulkan_context.surface,
          vulkan_context.allocator);
    }

    vkDestroyInstance(vulkan_context.instance, vulkan_context.allocator);
    ilog("vulkan instance destroyed");
}