#pragma once

#include "renderer/vulkan/types.h"

bool material_shader_create(Texture *shader_diffuse, VulkanShader *out_shader);

void material_shader_destroy(VulkanShader *shader);

void material_shader_use(VulkanShader *shader);

void material_shader_update_global_state(
  VulkanShader *shader, double delta_time);

void material_shader_update_object(
  VulkanShader *shader, GeometryRenderData data);

bool material_shader_acquire_resources(
  VulkanShader *shader, unsigned int *out_object_id);
void material_shader_release_resources(
  VulkanShader *shader, unsigned int object_id);
