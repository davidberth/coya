#include "device.h"
#include "core/logger.h"
#include "core/memory.h"
#include "renderer/vulkan/util.h"

extern VulkanContext vulkan_context;

typedef struct phyiscal_device_requirements {
    bool graphics;
    bool present;
    bool compute;
    bool transfer;

    const char **device_extension_names;
    bool discrete_gpu;
} PhysicalDeviceRequirements;

bool select_physical_device() {

    unsigned int physical_device_count = 0;
    vk_check(vkEnumeratePhysicalDevices(vulkan_context.instance,
                                        &physical_device_count, nullptr));
    if (physical_device_count == 0) {
        elog("failed to find GPUs with Vulkan support");
        return false;
    }

    ilog("found %d physical devices", physical_device_count);

    VkPhysicalDevice *physical_devices =
        oalloc(sizeof(VkPhysicalDevice) * physical_device_count,
               MEMORY_CATEGORY_VULKAN);

    vk_check(vkEnumeratePhysicalDevices(
        vulkan_context.instance, &physical_device_count, physical_devices));

    ofree(physical_devices);
    return true;
}

bool vulkan_device_create() { return select_physical_device(); }

void vulkan_device_cleanup() {}