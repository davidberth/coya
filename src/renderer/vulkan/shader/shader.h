#pragma once

#include "renderer/vulkan/types.h"

bool shader_create(Texture *shader_diffuse, VulkanShader *out_shader);

void shader_destroy(VulkanShader *shader);

void shader_use(VulkanShader *shader);

void vulkan_shader_update_global_state(VulkanShader *shader, double delta_time);

void vulkan_shader_update_object(VulkanShader *shader, GeometryRenderData data);

bool vulkan_shader_acquire_resources(
  VulkanShader *shader, unsigned int *out_object_id);
void vulkan_shader_release_resources(
  VulkanShader *shader, unsigned int object_id);
