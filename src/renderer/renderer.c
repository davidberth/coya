#include <stdio.h>
#include <stdlib.h>
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
        malloc(sizeof(VkExtensionProperties) * extension_count);
    vkEnumerateInstanceExtensionProperties(NULL, &extension_count, extensions);

    printf("available extensions:\n");

    for (uint32_t i = 0; i < extension_count; i++) {
        printf("%s\n", extensions[i].extensionName);
    }

    free(extensions);
}