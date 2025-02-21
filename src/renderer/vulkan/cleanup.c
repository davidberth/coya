#include "core/logger.h"
#include "renderer/vulkan/types.h"
#include "renderer/vulkan/device.h"
#include "renderer/vulkan/swapchain.h"
#include "renderer/vulkan/renderpass.h"
#include <vulkan/vulkan_core.h>

extern VulkanContext vulkan_context;

void renderer_cleanup() {
#if defined(_DEBUG)
    dlog("cleaning up the vulkan debugger");
    if (vulkan_context.debug_messenger) {

        PFN_vkDestroyDebugUtilsMessengerEXT func =
          (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
            vulkan_context.instance, "vkDestroyDebugUtilsMessengerEXT");

        func(vulkan_context.instance, vulkan_context.debug_messenger,
          vulkan_context.allocator);
    }
#endif

    ilog("clearning up the main renderpass");
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