#include "renderpass.h"
#include "renderer/vulkan/util.h"
#include <vulkan/vulkan_core.h>

extern VulkanContext vulkan_context;

void vulkan_renderpass_create(VulkanRenderpass *out_renderpass, float x,
  float y, float w, float h, float r, float g, float b, float a, float depth,
  unsigned int stencil) {

    out_renderpass->x = x;
    out_renderpass->y = y;
    out_renderpass->w = w;
    out_renderpass->h = h;
    out_renderpass->r = r;
    out_renderpass->g = g;
    out_renderpass->b = b;
    out_renderpass->a = a;
    out_renderpass->depth = depth;
    out_renderpass->stencil = stencil;

    VkSubpassDescription subpass = {};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;

    // attachments
    constexpr unsigned int attachment_description_count = 2;
    VkAttachmentDescription
      attachment_descriptions[attachment_description_count];

    // color
    VkAttachmentDescription color_attachment;
    color_attachment.format = vulkan_context.swapchain.image_format.format;
    color_attachment.samples = VK_SAMPLE_COUNT_1_BIT;
    color_attachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    color_attachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    color_attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    color_attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    color_attachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    color_attachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
    color_attachment.flags = 0;

    attachment_descriptions[0] = color_attachment;

    VkAttachmentReference color_attachment_reference;
    color_attachment_reference.attachment = 0;
    color_attachment_reference.layout =
      VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &color_attachment_reference;

    // depth (if there is one)
    VkAttachmentDescription depth_attachment = {};
    depth_attachment.format = vulkan_context.device.depth_format;
    depth_attachment.samples = VK_SAMPLE_COUNT_1_BIT;
    depth_attachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    depth_attachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    depth_attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    depth_attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    depth_attachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    depth_attachment.finalLayout =
      VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    attachment_descriptions[1] = depth_attachment;

    VkAttachmentReference depth_attachment_reference;
    depth_attachment_reference.attachment = 1;
    depth_attachment_reference.layout =
      VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    // TODO:other attachment types(input, resolve, preserve)

    subpass.inputAttachmentCount = 0;
    subpass.pInputAttachments = 0;

    subpass.pResolveAttachments = 0;

    subpass.preserveAttachmentCount = 0;
    subpass.pPreserveAttachments = 0;

    VkSubpassDependency dependency;
    dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
    dependency.dstSubpass = 0;
    dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.srcAccessMask = 0;
    dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT |
                               VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
    dependency.dependencyFlags = 0;

    VkRenderPassCreateInfo render_pass_create_info = {
      VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO};
    render_pass_create_info.attachmentCount = attachment_description_count;
    render_pass_create_info.pAttachments = attachment_descriptions;
    render_pass_create_info.subpassCount = 1;
    render_pass_create_info.pSubpasses = &subpass;
    render_pass_create_info.dependencyCount = 1;
    render_pass_create_info.pDependencies = &dependency;
    render_pass_create_info.pNext = 0;
    render_pass_create_info.flags = 0;

    vk_check(vkCreateRenderPass(vulkan_context.device.logical_device,
      &render_pass_create_info, vulkan_context.allocator,
      &out_renderpass->handle));
}

void vulkan_renderpass_destroy(VulkanRenderpass *renderpass) {
    if (renderpass && renderpass->handle) {
        vkDestroyRenderPass(vulkan_context.device.logical_device,
          renderpass->handle, vulkan_context.allocator);
        renderpass->handle = 0;
    }
}

void vulkan_renderpass_begin(VulkanCommandBuffer *command_buffer,
  VulkanRenderpass *renderpass, VkFramebuffer frame_buffer) {

    VkRenderPassBeginInfo begin_info = {
      VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO};
    begin_info.renderPass = renderpass->handle;
    begin_info.framebuffer = frame_buffer;
    begin_info.renderArea.offset.x = renderpass->x;
    begin_info.renderArea.offset.y = renderpass->y;
    begin_info.renderArea.extent.width = renderpass->w;
    begin_info.renderArea.extent.height = renderpass->h;

    VkClearValue clear_values[2];
    clear_values[0].color.float32[0] = renderpass->r;
    clear_values[0].color.float32[1] = renderpass->g;
    clear_values[0].color.float32[2] = renderpass->b;
    clear_values[0].color.float32[3] = renderpass->a;
    clear_values[1].depthStencil.depth = renderpass->depth;
    clear_values[1].depthStencil.stencil = renderpass->stencil;

    begin_info.clearValueCount = 2;
    begin_info.pClearValues = clear_values;

    vkCmdBeginRenderPass(
      command_buffer->handle, &begin_info, VK_SUBPASS_CONTENTS_INLINE);
    command_buffer->state = COMMAND_BUFFER_STATE_IN_RENDER_PASS;
}

void vulkan_renderpass_end(
  VulkanCommandBuffer *command_buffer, VulkanRenderpass *renderpass) {
    vkCmdEndRenderPass(command_buffer->handle);
    command_buffer->state = COMMAND_BUFFER_STATE_RECORDING;
}
