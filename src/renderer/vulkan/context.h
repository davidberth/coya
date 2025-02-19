#pragma once

#include <vulkan/vulkan.h>

// vulkan device
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

    VkQueue graphics_queue;
    VkQueue present_queue;
    VkQueue transfer_queue;
} VulkanDevice;

// swapchain support info
typedef struct {
    VkSurfaceCapabilitiesKHR capabilities;
    unsigned int format_count;
    VkSurfaceFormatKHR *formats;
    unsigned int present_mode_count;
    VkPresentModeKHR *present_modes;
} VulkanSwapchainSupportInfo;

// swapchain
typedef struct {
    VkSurfaceFormatKHR image_format;
    unsigned int max_frames_in_flight;
    VkSwapchainKHR handle;
    unsigned int image_count;
    VkImage *images;
    VkImageView *views;
} VulkanSwapchain;

// vulkan context
typedef struct {
    VkInstance instance;
    VulkanDevice device;
    VkSurfaceKHR surface;
    VkPhysicalDeviceProperties physical_device_properties;
    VkPhysicalDeviceFeatures physical_device_features;
#if defined(_DEBUG)
    VkDebugUtilsMessengerEXT debug_messenger;
#endif
} VulkanContext;
