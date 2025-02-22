#pragma once
#include "types.h"

void vulkan_command_buffer_allocate(
  VkCommandPool pool, bool is_primary, VulkanCommandBuffer *out_command_buffer);

void vulkan_command_buffer_free(
  VkCommandPool pool, VulkanCommandBuffer *command_buffer);

void vulkan_command_buffer_begin(VulkanCommandBuffer *command_buffer,
  bool is_single_use, bool is_renderpass_continue, bool is_simulataneous_use);

void vulkan_command_buffer_end(VulkanCommandBuffer *command_buffer);

void vulkan_command_buffer_update_submitted(
  VulkanCommandBuffer *command_buffer);

void vulkan_command_buffer_reset(VulkanCommandBuffer *command_buffer);

void vulkan_command_buffer_allocate_and_begin_single_use(
  VkCommandPool pool, VulkanCommandBuffer *out_command_buffer);

void vulkan_command_buffer_end_single_use(
  VkCommandPool pool, VulkanCommandBuffer *command_buffer, VkQueue queue);
