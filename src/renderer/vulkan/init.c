#include "core/logger.h"
#include "platform/util.h"
#include "renderer/vulkan/context.h"
#include "renderer/vulkan/util.h"
#include "renderer/vulkan/device.h"

extern VulkanContext vulkan_context;

constexpr unsigned int max_extensions = 6;

VKAPI_ATTR VkBool32 VKAPI_CALL
vk_debug_callback(VkDebugUtilsMessageSeverityFlagBitsEXT message_severity,
                  VkDebugUtilsMessageTypeFlagsEXT message_type,
                  const VkDebugUtilsMessengerCallbackDataEXT *callback_data,
                  void *user_data) {

    if (message_severity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT) {
        elog("validation layer: %s", callback_data->pMessage);
    } else if (message_severity >=
               VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) {
        wlog("validation layer: %s", callback_data->pMessage);
    } else {
        dlog("validation layer: %s", callback_data->pMessage);
    }

    return VK_FALSE;
}

bool renderer_init() {

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
#if defined(F_DEBUGF)
    extensions[extension_count++] = VK_EXT_DEBUG_UTILS_EXTENSION_NAME;
#endif

    ilog("vulkan extensions:");
    for (unsigned int i = 0; i < extension_count; i++) {
        ilog(" extension: %s", extensions[i]);
    }

    create_info.enabledExtensionCount = extension_count;
    create_info.ppEnabledExtensionNames = extensions;

#if defined(F_DEBUGBVBVBV)
    ilog("validation layers enabled");

    const char *validation_layers[1];
    validation_layers[0] = "VK_LAYER_KHRONOS_validation";

    create_info.enabledLayerCount = 1;
    create_info.ppEnabledLayerNames = validation_layers;
#else
    ilog("validation layers disabled");
    create_info.enabledLayerCount = 0;
    create_info.ppEnabledLayerNames = nullptr;
#endif

    vk_check(vkCreateInstance(&create_info, nullptr, &vulkan_context.instance));
    ilog("vulkan instance created");

#if defined(F_DEBUGVBVBVBV)
    unsigned int log_serverity =
        VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT |
        VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT;

    VkDebugUtilsMessengerCreateInfoEXT debug_create_info = {
        VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT};
    debug_create_info.messageSeverity = log_serverity;
    debug_create_info.messageType =
        VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
        VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
        VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    debug_create_info.pfnUserCallback = vk_debug_callback;

    PFN_vkCreateDebugUtilsMessengerEXT func =
        (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
            vulkan_context.instance, "vkCreateDebugUtilsMessengerEXT");
    vk_check(func(vulkan_context.instance, &debug_create_info, nullptr,
                  &vulkan_context.debug_messenger));
    dlog("vulkan debug messenger created");

#endif

    ilog("creating vulkan surface");
    if (!create_vulkan_surface()) {
        elog("failed to create vulkan surface");
        return false;
    }
    if (!vulkan_device_create()) {
        elog("failed to create vulkan device");
        return false;
    }
    ilog("vulkan surface created");

    dlog("vulkan renderer initialized");
    return true;
}