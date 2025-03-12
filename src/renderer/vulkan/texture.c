#include "renderer/renderer.h"
#include "renderer/vulkan/command_buffer.h"
#include "renderer/vulkan/types.h"
#include "core/memory.h"
#include <vulkan/vulkan_core.h>
#include "renderer/vulkan/buffer.h"
#include "renderer/vulkan/image.h"
#include "renderer/vulkan/util.h"
#include "core/logger.h"

void renderer_create_texture(const char *name, bool auto_release,
  unsigned int width, unsigned int height, unsigned short int channel_count,
  const unsigned char *pixels, bool has_alpha, Texture *out_texture) {
    out_texture->width = width;
    out_texture->height = height;
    out_texture->channel_count = channel_count;
    out_texture->generation = 0;

    // TODO: Use an allocator for this

    out_texture->internal_data = (VulkanTextureData *)oalloc(
      sizeof(VulkanTextureData), MEMORY_CATEGORY_TEXTURE);
    VulkanTextureData *data = (VulkanTextureData *)out_texture->internal_data;
    VkDeviceSize image_size = width * height * channel_count;

    VkFormat image_format = VK_FORMAT_R8G8B8A8_UNORM;
    VkBufferUsageFlags usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
    VkMemoryPropertyFlags memory_prop_flags =
      VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
      VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
    VulkanBuffer staging;
    vulkan_buffer_create(image_size, usage, memory_prop_flags, true, &staging);

    vulkan_buffer_load_data(&staging, 0, image_size, 0, pixels);

    vulkan_image_create(VK_IMAGE_TYPE_2D, width, height, image_format,
      VK_IMAGE_TILING_OPTIMAL,
      VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT |
        VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
      VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, true, VK_IMAGE_ASPECT_COLOR_BIT,
      &data->image);

    VulkanCommandBuffer temp_buffer;
    VkCommandPool pool = vulkan_context.device.graphics_command_pool;
    VkQueue queue = vulkan_context.device.graphics_queue;
    vulkan_command_buffer_allocate_and_begin_single_use(pool, &temp_buffer);

    vulkan_image_transition_layout(&temp_buffer, &data->image, image_format,
      VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);

    vulkan_image_copy_from_buffer(&data->image, staging.handle, &temp_buffer);

    vulkan_image_transition_layout(&temp_buffer, &data->image, image_format,
      VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
      VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

    vulkan_command_buffer_end_single_use(pool, &temp_buffer, queue);

    VkSamplerCreateInfo sampler_info = {VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO};
    sampler_info.magFilter = VK_FILTER_LINEAR;
    sampler_info.minFilter = VK_FILTER_LINEAR;
    sampler_info.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    sampler_info.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    sampler_info.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    sampler_info.anisotropyEnable = VK_TRUE;
    sampler_info.maxAnisotropy = 16;
    sampler_info.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
    sampler_info.unnormalizedCoordinates = VK_FALSE;
    sampler_info.compareEnable = VK_FALSE;
    sampler_info.compareOp = VK_COMPARE_OP_ALWAYS;
    sampler_info.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
    sampler_info.mipLodBias = 0.0f;
    sampler_info.minLod = 0.0f;
    sampler_info.maxLod = 0.0f;

    VkResult result = vkCreateSampler(vulkan_context.device.logical_device,
      &sampler_info, vulkan_context.allocator, &data->sampler);
    if (!vulkan_result_is_success(result)) {
        mlog("Failed to create sampler");
    }
    out_texture->has_alpha = has_alpha;
    ++out_texture->generation;
}

void renderer_destroy_texture(Texture *texture) {
    VulkanTextureData *data = (VulkanTextureData *)texture->internal_data;
    vulkan_image_destroy(&data->image);
    vkDestroySampler(vulkan_context.device.logical_device, data->sampler,
      vulkan_context.allocator);
    data->sampler = nullptr;

    ofree(texture->internal_data);
    texture->internal_data = nullptr;
}