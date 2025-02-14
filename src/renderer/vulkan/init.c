#include "core/logger.h"
#include "platform/util.h"
#include "renderer/vulkan/context.h"
#include "renderer/vulkan/util.h"

extern VulkanContext vulkan_context;

constexpr unsigned int max_extensions = 8;

bool init_renderer() {

    VkApplicationInfo app_info = {VK_STRUCTURE_TYPE_APPLICATION_INFO};
    app_info.pApplicationName = "Coya";
    app_info.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    app_info.pEngineName = "No Engine";
    app_info.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    app_info.apiVersion = VK_API_VERSION_1_4;

    VkInstanceCreateInfo create_info = {VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO};
    create_info.pApplicationInfo = &app_info;

    const char *extensions[max_extensions];
    unsigned int extension_count = 0;
    get_vulkan_platform_extensions(extensions, &extension_count);
    extensions[extension_count++] = VK_KHR_SURFACE_EXTENSION_NAME;
#if defined(_DEBUG)
    extensions[extension_count++] = VK_EXT_DEBUG_UTILS_EXTENSION_NAME;
#endif

    ilog("vulkan extensions:");
    for (unsigned int i = 0; i < extension_count; i++) {
        ilog(" extension: %s", extensions[i]);
    }

    create_info.enabledExtensionCount = extension_count;
    create_info.ppEnabledExtensionNames = extensions;

#if defined(_DEBUG)
    ilog("Validation layers enabled");

    const char *validation_layers[1];
    validation_layers[0] = "VK_LAYER_KHRONOS_validation";

    create_info.enabledLayerCount = 1;
    create_info.ppEnabledLayerNames = validation_layers;
#else
    ilog("Validation layers disabled");
    create_info.enabledLayerCount = 0;
    create_info.ppEnabledLayerNames = nullptr;
#endif

    vk_check(vkCreateInstance(&create_info, nullptr, &vulkan_context.instance));

    ilog("vulkan instance created");
    return true;
}