#pragma once

#include "renderer/vulkan/types.h"
#include "renderer/renderer.h"

bool shader_create(VulkanShader *out_shader);

void shader_destroy(VulkanShader *shader);

void shader_use(VulkanShader *shader);
