#include "renderer/vulkan/device.h"
#include "core/logger.h"
#include "core/memory.h"
#include "renderer/vulkan/util.h"
#include "renderer/vulkan/context.h"

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

    VkPhysicalDeviceProperties properties;
    VkPhysicalDeviceFeatures features;
    bool found_gpu = false;
    for (unsigned int i = 0; i < physical_device_count; i++) {

        vkGetPhysicalDeviceProperties(physical_devices[i], &properties);

        vkGetPhysicalDeviceFeatures(physical_devices[i], &features);
        if (properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
            found_gpu = true;
            vulkan_context.device.physical_device = physical_devices[i];
            break;
        }
    }
    ofree(physical_devices);
    if (!found_gpu) {
        elog("failed to find a suitable GPU");
        return false;
    }

    ilog("selected device:");
    ilog(" %s", properties.deviceName);
    ilog("Vulkan version: %d.%d.%d", VK_VERSION_MAJOR(properties.apiVersion),
         VK_VERSION_MINOR(properties.apiVersion),
         VK_VERSION_PATCH(properties.apiVersion));

    ilog("GPU driver version: %d.%d.%d",
         VK_VERSION_MAJOR(properties.driverVersion),
         VK_VERSION_MINOR(properties.driverVersion),
         VK_VERSION_PATCH(properties.driverVersion));

    vulkan_context.device.properties = properties;
    vulkan_context.device.features = features;

    VkPhysicalDeviceMemoryProperties memory;
    vkGetPhysicalDeviceMemoryProperties(vulkan_context.device.physical_device,
                                        &memory);

    for (unsigned int i = 0; i < memory.memoryTypeCount; i++) {
        float memory_size_gb =
            (((float)memory.memoryHeaps[memory.memoryTypes[i].heapIndex].size) /
             1073741824.0f);
        if (memory.memoryHeaps[i].flags & VK_MEMORY_HEAP_DEVICE_LOCAL_BIT) {
            ilog("found device local memory: %.2f GB", memory_size_gb);
        }
    }

    vulkan_context.device.memory = memory;

    unsigned int queue_family_count = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(
        vulkan_context.device.physical_device, &queue_family_count, nullptr);

    VkQueueFamilyProperties *queue_families = (VkQueueFamilyProperties *)oalloc(
        sizeof(VkQueueFamilyProperties) * queue_family_count,
        MEMORY_CATEGORY_VULKAN);

    vkGetPhysicalDeviceQueueFamilyProperties(
        vulkan_context.device.physical_device, &queue_family_count,
        queue_families);

    vulkan_context.device.graphics_queue_index = -1;
    vulkan_context.device.present_queue_index = -1;
    vulkan_context.device.compute_queue_index = -1;
    vulkan_context.device.transfer_queue_index = -1;

    int compute_taken = 99;
    int transfer_taken = 99;
    int present_taken = 99;
    for (unsigned int i = 0; i < queue_family_count; i++) {
        int queues_taken = 0;

        if (queue_families[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            vulkan_context.device.graphics_queue_index = i;
            ilog("found graphics queue family index: %d", i);
            queues_taken++;
        }
        if (queue_families[i].queueFlags & VK_QUEUE_COMPUTE_BIT) {
            if (queues_taken < compute_taken) {
                vulkan_context.device.compute_queue_index = i;
                compute_taken = queues_taken;
            }
            ilog("found compute queue family index: %d", i);
            queues_taken++;
        }
        if (queue_families[i].queueFlags & VK_QUEUE_TRANSFER_BIT) {
            if (queues_taken < transfer_taken) {
                vulkan_context.device.transfer_queue_index = i;
                transfer_taken = queues_taken;
            }
            ilog("found transfer queue family index: %d", i);
            queues_taken++;
        }
        VkBool32 present_support = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(
            vulkan_context.device.physical_device, i, vulkan_context.surface,
            &present_support);
        if (present_support) {
            if (queues_taken < present_taken) {
                vulkan_context.device.present_queue_index = i;
                present_taken = queues_taken;
            }
            ilog("found present queue family index: %d", i);
        }
    }

    ofree(queue_families);

    ilog("selected queue family indices:");
    ilog(" graphics: %d", vulkan_context.device.graphics_queue_index);
    ilog(" present: %d", vulkan_context.device.present_queue_index);
    ilog(" compute: %d", vulkan_context.device.compute_queue_index);
    ilog(" transfer: %d", vulkan_context.device.transfer_queue_index);

    return true;
}

bool vulkan_device_create() {
    if (!select_physical_device())
        return false;
    // if (!create_logical_device()) return false
    return true;
}

void vulkan_device_cleanup() { ilog("cleaning up Vulkan"); }