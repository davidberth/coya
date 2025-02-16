#include <vulkan/vulkan.h>

typedef struct {
    VkInstance instance;

#if defined(_DEBUG)
    VkDebugUtilsMessengerEXT debug_messenger;
#endif
} VulkanContext;
