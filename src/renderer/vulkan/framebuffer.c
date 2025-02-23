#include "framebuffer.h"
#include "util.h"

#include "core/memory.h"

extern VulkanContext vulkan_context;

void vulkan_framebuffer_create(VulkanRenderpass *renderpass, unsigned int width,
  unsigned int height, unsigned int attachment_count, VkImageView *attachments,
  VulkanFramebuffer *out_framebuffer) {

    out_framebuffer->attachments =
      oalloc(sizeof(VkImageView) * attachment_count, MEMORY_CATEGORY_VULKAN);
    for (unsigned int i = 0; i < attachment_count; i++) {
        out_framebuffer->attachments[i] = attachments[i];
    }

    out_framebuffer->renderpass = renderpass;
    out_framebuffer->attachment_count = attachment_count;

    VkFramebufferCreateInfo framebuffer_create_info = {
      VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO};
    framebuffer_create_info.renderPass = renderpass->handle;
    framebuffer_create_info.attachmentCount = attachment_count;
    framebuffer_create_info.pAttachments = attachments;
    framebuffer_create_info.width = width;
    framebuffer_create_info.height = height;
    framebuffer_create_info.layers = 1;

    vk_check(vkCreateFramebuffer(vulkan_context.device.logical_device,
      &framebuffer_create_info, vulkan_context.allocator,
      &out_framebuffer->handle));
}

void vulkan_framebuffer_destroy(VulkanFramebuffer *framebuffer) {
    vkDestroyFramebuffer(vulkan_context.device.logical_device,
      framebuffer->handle, vulkan_context.allocator);
    if (framebuffer->attachments) {
        ofree(framebuffer->attachments);
        framebuffer->attachments = nullptr;
    }
    framebuffer->handle = 0;
    framebuffer->attachment_count = 0;
    framebuffer->renderpass = nullptr;
}