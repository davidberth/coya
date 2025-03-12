#pragma once

#include "renderer/vulkan/types.h"

bool shader_create(VulkanShader *out_shader);

void shader_destroy(VulkanShader *shader);

void shader_use(VulkanShader *shader);

void vulkan_shader_update_global_state(VulkanShader *shader, float delta_time);

void vulkan_shader_update_object(VulkanShader *shader, GeometryRenderData data);

bool vulkan_object_shader_acquire_resources(
  VulkanShader *shader, unsigned int *out_object_id);
void vulkan_oject_shader_release_resources(
  VulkanShader *shader, unsigned int object_id);
