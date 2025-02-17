#pragma once

#include <vulkan/vulkan.h>

typedef struct {
    VkPhysicalDevice physical_device;
    VkDevice logical_device;
} VulkanDevice;

typedef struct {
    VkSurfaceCapabilitiesKHR capabilities;
    unsigned int format_count;
    VkSurfaceFormatKHR *formats;
    unsigned int present_mode_count;
    VkPresentModeKHR *present_modes;
} VulkanSwapchainSupportInfo;

typedef struct {
    VkInstance instance;
    VulkanDevice device;
    VkSurfaceKHR surface;
#if defined(_DEBUG)
    VkDebugUtilsMessengerEXT debug_messenger;
#endif
} VulkanContext;
