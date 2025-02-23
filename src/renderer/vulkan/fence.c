#include "fence.h"
#include "util.h"
#include "core/logger.h"

extern VulkanContext vulkan_context;

void vulkan_fence_create(bool create_signaled, VulkanFence *out_fence) {
    out_fence->is_signaled = create_signaled;
    VkFenceCreateInfo fence_create_info = {VK_STRUCTURE_TYPE_FENCE_CREATE_INFO};
    if (create_signaled) {
        fence_create_info.flags = VK_FENCE_CREATE_SIGNALED_BIT;
    }
    vk_check(vkCreateFence(vulkan_context.device.logical_device,
      &fence_create_info, vulkan_context.allocator, &out_fence->handle));
}

void vulkan_fence_destroy(VulkanFence *fence) {
    if (fence->handle) {
        vkDestroyFence(vulkan_context.device.logical_device, fence->handle,
          vulkan_context.allocator);
        fence->handle = 0;
    }
    fence->is_signaled = false;
}

bool vulkan_fence_wait(VulkanFence *fence, uint64_t timeout_ns) {
    if (!fence->is_signaled) {
        VkResult result = vkWaitForFences(vulkan_context.device.logical_device,
          1, &fence->handle, true, timeout_ns);
        switch (result) {
        case VK_SUCCESS:
            fence->is_signaled = true;
            return true;
        case VK_TIMEOUT:
            wlog("fence wait timed out");
            break;
        case VK_ERROR_DEVICE_LOST:
            elog("device lost during fence wait");
            break;
        case VK_ERROR_OUT_OF_HOST_MEMORY:
            elog("out of host memory during fence wait");
            break;
        case VK_ERROR_OUT_OF_DEVICE_MEMORY:
            elog("out of device memory during fence wait");
            break;
        default:
            elog("unknown error during fence wait");
            break;
        }
    } else {
        // already signaled so no need to wait
        return true;
    }

    return false;
}

void vulkan_fence_reset(VulkanFence *fence) {
    if (fence->is_signaled) {
        vkResetFences(vulkan_context.device.logical_device, 1, &fence->handle);
        fence->is_signaled = false;
    }
}
