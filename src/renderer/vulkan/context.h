#pragma once

#include <vulkan/vulkan.h>

typedef struct {
    VkPhysicalDevice physical_device;
    VkDevice logical_device;
} VulkanDevice;

typedef struct {
    VkInstance instance;
    VulkanDevice device;
    VkSurfaceKHR surface;
#if defined(_DEBUG)
    VkDebugUtilsMessengerEXT debug_messenger;
#endif
} VulkanContext;
