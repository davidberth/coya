#pragma once

#include <assert.h>
#include <core/logger.h>

#include <vulkan/vulkan.h>
#include <vulkan/vk_enum_string_helper.h>

#define vk_check(expr)                                                         \
    {                                                                          \
        VkResult result = expr;                                                \
        if (result != VK_SUCCESS) {                                            \
            elog("vulkan error: %s", string_VkResult(result));                 \
            assert(false);                                                     \
        }                                                                      \
    }

bool vulkan_result_is_success(VkResult result);
