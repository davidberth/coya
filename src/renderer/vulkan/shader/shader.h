#pragma once

#include "renderer/vulkan/types.h"

bool shader_create(VulkanShader *out_shader);

void shader_destroy(VulkanShader *shader);

void shader_use(VulkanShader *shader);

void vulkan_object_shader_update_global_state(VulkanShader *shader);