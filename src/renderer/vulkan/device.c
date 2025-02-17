#include "device.h"
#include "core/logger.h"
#include "core/memory.h"
#include "renderer/vulkan/util.h"

extern VulkanContext vulkan_context;

typedef struct {
    unsigned int graphics_family_index;
    unsigned int present_family_index;
    unsigned int compute_family_index;
    unsigned int transfer_family_index;
} VulkanPhysicalDeviceQueueFamilyInfo;

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

    unsigned int selected_device_number = 0;
    for (unsigned int i = 0; i < physical_device_count; i++) {
        VkPhysicalDeviceProperties properties;
        vkGetPhysicalDeviceProperties(physical_devices[i], &properties);

        VkPhysicalDeviceFeatures features;
        vkGetPhysicalDeviceFeatures(physical_devices[i], &features);
        if (properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
            ilog("found discrete GPU: %s", properties.deviceName);
            ilog("selecting this device");
            selected_device_number = i;
        } else if (properties.deviceType ==
                   VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU) {
            ilog("found integrated GPU: %s", properties.deviceName);
        } else if (properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_CPU) {
            ilog("found CPU: %s", properties.deviceName);
        } else if (properties.deviceType ==
                   VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU) {
            ilog("found virtual GPU: %s", properties.deviceName);
        } else {
            ilog("found unknown device type: %s", properties.deviceName);
        }

        ilog("device %d: %s", i, properties.deviceName);
    }

    ofree(physical_devices);
    return true;
}

bool vulkan_device_create() { return select_physical_device(); }

void vulkan_device_cleanup() {}