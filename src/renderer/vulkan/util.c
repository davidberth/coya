#include <vulkan/vulkan.h>
#include "renderer/vulkan/types.h"

extern VulkanContext vulkan_context;

bool vulkan_result_is_success(VkResult result) {
    switch (result) {
    case VK_SUCCESS:
    case VK_NOT_READY:
    case VK_TIMEOUT:
    case VK_EVENT_SET:
    case VK_INCOMPLETE:
    case VK_SUBOPTIMAL_KHR:
    case VK_THREAD_IDLE_KHR:
    case VK_OPERATION_DEFERRED_KHR:
    case VK_OPERATION_NOT_DEFERRED_KHR:
    case VK_PIPELINE_COMPILE_REQUIRED_EXT:
        return true;
    default:
        return false;
    }
    return false;
}

float renderer_get_delta_time() { return vulkan_context.delta_time; }