#pragma once

#include <vulkan/vulkan.h>

// image
typedef struct {
    VkImage handle;
    VkDeviceMemory memory;
    VkImageView view;
    unsigned int width;
    unsigned int height;
} VulkanImage;

typedef enum {
    READY,
    RECORDING,
    IN_RENDER_PASS,
    RECORDING_ENDED,
    SUBMITTED,
    NOT_ALLOCATED
} VulkanRenderPassState;

typedef struct {
    VkRenderPass handle;
    float x, y, w, h;
    float r, g, b, a;

    float depth;
    unsigned int stencil;

    VulkanRenderPassState state;
} VulkanRenderpass;

typedef struct {
    VkFramebuffer handle;
    unsigned int attachment_count;
    VkImageView *attachments;
    VulkanRenderpass *renderpass;
} VulkanFramebuffer;

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

    VkCommandPool graphics_command_pool;

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

    VulkanImage depth_attachment;

    VulkanFramebuffer *framebuffers;
} VulkanSwapchain;

typedef enum {
    COMMAND_BUFFER_STATE_READY,
    COMMAND_BUFFER_STATE_RECORDING,
    COMMAND_BUFFER_STATE_IN_RENDER_PASS,
    COMMAND_BUFFER_STATE_RECORDING_ENDED,
    COMMAND_BUFFER_STATE_SUBMITTED,
    COMMAND_BUFFER_STATE_NOT_ALLOCATED
} VulkanCommandBufferState;

typedef struct {
    VkCommandBuffer handle;
    VulkanCommandBufferState state;
} VulkanCommandBuffer;

typedef struct {
    VkFence handle;
    bool is_signaled;
} VulkanFence;

// shader stage
typedef struct {
    VkShaderModuleCreateInfo create_info;
    VkShaderModule handle;
    VkPipelineShaderStageCreateInfo stage_info;
} VulkanShaderStage;

// pipeline
typedef struct {
    VkPipeline handle;
    VkPipelineLayout layout;
} VulkanPipeline;

constexpr int object_shader_stage_count = 2;
// shader
typedef struct {
    VulkanShaderStage stages[object_shader_stage_count];
    VulkanPipeline pipeline;
} VulkanShader;

// vulkan context
typedef struct {
    unsigned int framebuffer_width;
    unsigned int framebuffer_height;

    bool framebuffer_resize_needed;

    VkInstance instance;
    VulkanDevice device;
    VkSurfaceKHR surface;
    VulkanSwapchain swapchain;

    VkAllocationCallbacks *allocator;

    unsigned int image_index;
    unsigned int current_frame;

    bool recreating_swapchain;
    VulkanRenderpass main_renderpass;

    VulkanCommandBuffer *graphics_command_buffers;

    VkSemaphore *image_available_semaphores;
    VkSemaphore *queue_complete_semaphores;

    unsigned int in_flight_fence_count;
    VulkanFence *in_flight_fences;
    VulkanFence **images_in_flight;

    int (*find_memory_index)(
      unsigned int type_filter, unsigned int property_flags);

#if defined(_DEBUG)
    VkDebugUtilsMessengerEXT debug_messenger;
#endif
} VulkanContext;
