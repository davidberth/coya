#include <vulkan/vulkan.h>

bool vulkan_result_is_success(VkResult result) { return result == VK_SUCCESS; }
