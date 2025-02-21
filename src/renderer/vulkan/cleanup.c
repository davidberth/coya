#include "renderer/vulkan/types.h"
#include "core/logger.h"
#include "core/memory.h"
#include "renderer/vulkan/device.h"
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

    dlog("cleaning up device");
    vulkan_device_cleanup();

    dlog("cleaning up the surface");
    if (vulkan_context.surface) {
        vkDestroySurfaceKHR(vulkan_context.instance, vulkan_context.surface,
                            vulkan_context.allocator);
    }

    dlog("cleaning up the swapchain info");
    ofree(vulkan_context.device.swapchain_support.formats);
    ofree(vulkan_context.device.swapchain_support.present_modes);

    vkDestroyInstance(vulkan_context.instance, vulkan_context.allocator);
    dlog("vulkan instance destroyed");
}