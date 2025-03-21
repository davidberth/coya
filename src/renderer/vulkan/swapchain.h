#pragma once

#include "renderer/vulkan/types.h"

void vulkan_swapchain_create(unsigned int width, unsigned int height,
  VulkanSwapchain *out_swapchain, bool vsync);

void vulkan_swapchain_recreate(unsigned int width, unsigned int height,
  VulkanSwapchain *swapchain, bool vsyc);

void vulkan_swapchain_destroy(VulkanSwapchain *swapchain);

bool vulkan_swapchain_acquire_next_image_index(VulkanSwapchain *swapchain,
  unsigned int timeout_ns, VkSemaphore image_available_semaphore, VkFence fence,
  unsigned int *out_image_index);

void vulkan_swapchain_present(VulkanSwapchain *swapchain,
  VkQueue grahpics_queue, VkQueue present_queue,
  VkSemaphore render_complete_semaphore, unsigned int present_image_index);