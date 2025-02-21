#include <windows.h>
#include "renderer/vulkan/types.h"
#include "core/logger.h"
#include <vulkan/vulkan_win32.h>

// TODO: move this into a struct to hold all windows related context
extern HWND window_handle;
extern HINSTANCE window_hinstance;

extern VulkanContext vulkan_context;

void get_vulkan_platform_extensions(const char **extensions,
                                    unsigned int *count) {
    *count = 0;
    extensions[(*count)++] = "VK_KHR_win32_surface";
}

bool create_vulkan_surface() {
    VkWin32SurfaceCreateInfoKHR create_info = {
        VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR};
    create_info.hinstance = window_hinstance;
    create_info.hwnd = window_handle;

    if (vkCreateWin32SurfaceKHR(vulkan_context.instance, &create_info, nullptr,
                                &vulkan_context.surface) != VK_SUCCESS) {
        elog("failed to create window surface");
        return false;
    }

    return true;
}