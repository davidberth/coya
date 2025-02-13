#include <core/logger.h>
#include <core/memory.h>
#include <vulkan/vulkan.h>

void init_vulkan() {

    VkApplicationInfo app_info = {};
    app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    app_info.pApplicationName = "Hello Triangle";
    app_info.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    app_info.pEngineName = "No Engine";
    app_info.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    app_info.apiVersion = VK_API_VERSION_1_0;

    VkInstanceCreateInfo create_info = {};
    create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    create_info.pApplicationInfo = &app_info;

    uint32_t extension_count = 0;
    vkEnumerateInstanceExtensionProperties(NULL, &extension_count, NULL);

    VkExtensionProperties *extensions =
        oalloc(sizeof(VkExtensionProperties) * extension_count,
               MEMORY_CATEGORY_VULKAN);
    vkEnumerateInstanceExtensionProperties(NULL, &extension_count, extensions);

    ilog("available extensions:");

    for (uint32_t i = 0; i < extension_count; i++) {
        ilog("%s", extensions[i].extensionName);
    }
    ofree(extensions);
}