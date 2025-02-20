#pragma once

#include <vulkan/vulkan.h>

// swapchain support info
typedef struct {
    VkSurfaceCapabilitiesKHR capabilities;
    unsigned int format_count;
    VkSurfaceFormatKHR *formats;
    unsigned int present_mode_count;
    VkPresentModeKHR *present_modes;
} VulkanSwapchainSupportInfo;

// vulkan device
typedef struct {
    VkPhysicalDevice physical_device;
    VkDevice logical_device;
    VkPhysicalDeviceProperties properties;
    VkPhysicalDeviceFeatures features;
    VkPhysicalDeviceMemoryProperties memory;

    VulkanSwapchainSupportInfo swapchain_support;

    VkFormat depth_format; 

    int graphics_queue_index;
    int present_queue_index;
    int transfer_queue_index;
    int compute_queue_index;

    VkQueue graphics_queue;
    VkQueue present_queue;
    VkQueue transfer_queue;
} VulkanDevice;

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
    unsigned int framebuffer_width;
    unsigned int framebuffer_height;

    VkInstance instance;
    VulkanDevice device;
    VkSurfaceKHR surface;
    VulkanSwapchain swapchain;
    unsigned int image_index;
    unsigned int current_frame;

    bool recreating_swapchain;

#if defined(_DEBUG)
    VkDebugUtilsMessengerEXT debug_messenger;
#endif
} VulkanContext;
