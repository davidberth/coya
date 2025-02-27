#pragma once

#include "types.h"

bool create_shader_module(const char *name, const char *type_str,
  VkShaderStageFlagBits shader_stage_flag, unsigned int stage_index,
  VulkanShaderStage *shader_stages);

