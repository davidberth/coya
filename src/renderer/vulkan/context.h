#pragma once

#include <vulkan/vulkan.h>

typedef struct {
    VkPhysicalDevice physical_device;
    VkDevice logical_device;
    VkPhysicalDeviceProperties properties;
    VkPhysicalDeviceFeatures features;
    VkPhysicalDeviceMemoryProperties memory;

    int graphics_queue_index;
    int present_queue_index;
    int transfer_queue_index;
    int compute_queue_index;

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
    VkPhysicalDeviceProperties physical_device_properties;
    VkPhysicalDeviceFeatures physical_device_features;
#if defined(_DEBUGF)
    VkDebugUtilsMessengerEXT debug_messenger;
#endif
} VulkanContext;
