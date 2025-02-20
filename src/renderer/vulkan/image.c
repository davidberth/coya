#include "image.h"

#include "core/memory.h"
#include "core/logger.h"
#include "renderer/vulkan/util.h"
#include "renderer/vulkan/context.h"

extern VulkanContext vulkan_context;

void vulkan_image_create(VkImageType image_type, unsigned int width,
                         unsigned int height, VkFormat format,
                         VkImageTiling tiling, VkImageUsageFlags usage,
                         VkMemoryPropertyFlags memory_flags, bool create_view,
                         VkImageAspectFlags view_aspect_flags,
                         VulkanImage *out_image) {
    out_image->width = width;
    out_image->height = height;

    VkImageCreateInfo image_info = {VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO};
    image_info.imageType = image_type;
    image_info.extent.width = width;
    image_info.extent.height = height;
    image_info.extent.depth = 1; // TODO: configurable depth
    image_info.mipLevels = 1;    // TODO: support mip mapping (maybe)
    image_info.arrayLayers = 1;
    image_info.format = format;
    image_info.tiling = tiling;
    image_info.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    image_info.usage = usage;
    image_info.samples =
        VK_SAMPLE_COUNT_1_BIT; // TODO: configurable sample count
    image_info.sharingMode =
        VK_SHARING_MODE_EXCLUSIVE; // TODO: configurable sharing mode

    vk_check(vkCreateImage(vulkan_context.device.logical_device, &image_info,
                           nullptr, &out_image->handle));

    VkMemoryRequirements memory_requirements;
    vkGetImageMemoryRequirements(vulkan_context.device.logical_device,
                                 out_image->handle, &memory_requirements);

    int memory_type = vulkan_context.find_memory_index(
        memory_requirements.memoryTypeBits, memory_flags);
    if (memory_type == -1) {
        flog("failed to find suitable memory type for image");
        return;
    }

    VkMemoryAllocateInfo alloc_info = {VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO};
    alloc_info.allocationSize = memory_requirements.size;
    alloc_info.memoryTypeIndex = memory_type;
    vk_check(vkAllocateMemory(vulkan_context.device.logical_device, &alloc_info,
                              nullptr, &out_image->memory));
    vk_check(vkBindImageMemory(vulkan_context.device.logical_device,
                               out_image->handle, out_image->memory, 0));
}

void vulkan_image_view_create(VkFormat format, VulkanImage *image,
                              VkImageAspectFlags aspect_flags) {}

void vulkan_image_destroy(VulkanImage *image) {}