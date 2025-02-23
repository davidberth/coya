#pragma once

#include "types.h"

void vulkan_fence_create(bool create_signaled, VulkanFence *out_fence);

void vulkan_fence_destroy(VulkanFence *fence);

bool vulkan_fence_wait(VulkanFence *fence, uint64_t timeout_ns);

void vulkan_fence_reset(VulkanFence *fence);
