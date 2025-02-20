#include "swapchain.h"

#include "core/logger.h"
#include "renderer/vulkan/context.h"
#include "renderer/vulkan/device.h"
#include "renderer/vulkan/util.h"
#include "core/util.h"
#include "core/memory.h"
#include <vulkan/vulkan_core.h>

extern VulkanContext vulkan_context;

void create(unsigned width, unsigned height, VulkanSwapchain *swapchain) {
    ilog("creating swapchain");

    VkExtent2D swapchain_extent = {width, height};
    swapchain->max_frames_in_flight = 2;

    // create a temporary reference to swapchain support info
    VulkanSwapchainSupportInfo *support_info =
        &vulkan_context.device.swapchain_support;

    bool found = false;
    for (unsigned int i = 0; i < support_info->format_count; i++) {
        VkSurfaceFormatKHR format = support_info->formats[i];
        // check if format is desired one
        if (format.format == VK_FORMAT_B8G8R8A8_UNORM &&
            format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
            swapchain->image_format = format;
            found = true;
            break;
        }
    }
    if (!found) {
        swapchain->image_format = support_info->formats[0];
    }

    VkPresentModeKHR present_mode = VK_PRESENT_MODE_FIFO_KHR;
    for (unsigned int i = 0; i < support_info->present_mode_count; i++) {
        if (support_info->present_modes[i] == VK_PRESENT_MODE_MAILBOX_KHR) {
            present_mode = VK_PRESENT_MODE_MAILBOX_KHR;
            break;
        }
    }

    vulkan_device_query_swapchain_support();
    if (support_info->capabilities.currentExtent.width != UINT32_MAX) {
        swapchain_extent = support_info->capabilities.currentExtent;
    }
    VkExtent2D min = support_info->capabilities.minImageExtent;
    VkExtent2D max = support_info->capabilities.maxImageExtent;
    swapchain_extent.width =
        oclamp(swapchain_extent.width, min.width, max.width);
    swapchain_extent.height =
        oclamp(swapchain_extent.height, min.height, max.height);

    unsigned int image_count = support_info->capabilities.minImageCount + 1;
    if (support_info->capabilities.maxImageCount > 0 &&
        image_count > support_info->capabilities.maxImageCount) {
        image_count = support_info->capabilities.maxImageCount;
    }

    VkSwapchainCreateInfoKHR create_info = {
        VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR};
    create_info.surface = vulkan_context.surface;
    create_info.minImageCount = image_count;
    create_info.imageFormat = swapchain->image_format.format;
    create_info.imageColorSpace = swapchain->image_format.colorSpace;
    create_info.imageExtent = swapchain_extent;
    create_info.imageArrayLayers = 1;
    create_info.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    if (vulkan_context.device.graphics_queue_index !=
        vulkan_context.device.present_queue_index) {
        unsigned int queue_family_indices[] = {
            vulkan_context.device.graphics_queue_index,
            vulkan_context.device.present_queue_index};
        create_info.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        create_info.queueFamilyIndexCount = 2;
        create_info.pQueueFamilyIndices = queue_family_indices;
    } else {
        create_info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        create_info.queueFamilyIndexCount = 0;
        create_info.pQueueFamilyIndices = nullptr;
    }

    create_info.preTransform = support_info->capabilities.currentTransform;
    create_info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    create_info.presentMode = present_mode;
    create_info.clipped = VK_TRUE;
    create_info.oldSwapchain = VK_NULL_HANDLE;

    vk_check(vkCreateSwapchainKHR(vulkan_context.device.logical_device,
                                  &create_info, nullptr, &swapchain->handle));
    vulkan_context.current_frame = 0;
    swapchain->image_count = 0;
    vk_check(vkGetSwapchainImagesKHR(vulkan_context.device.logical_device,
                                     swapchain->handle, &swapchain->image_count,
                                     0));

    if (!swapchain->images) {
        swapchain->images = (VkImage *)oalloc(
            sizeof(VkImage) * swapchain->image_count, MEMORY_CATEGORY_VULKAN);
    }
    if (!swapchain->views) {
        swapchain->views =
            (VkImageView *)oalloc(sizeof(VkImageView) * swapchain->image_count,
                                  MEMORY_CATEGORY_VULKAN);
    }
    vk_check(vkGetSwapchainImagesKHR(vulkan_context.device.logical_device,
                                     swapchain->handle, &swapchain->image_count,
                                     swapchain->images));

    // views
    for (unsigned int i = 0; i < swapchain->image_count; ++i) {
        VkImageViewCreateInfo view_info = {
            VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO};
        view_info.image = swapchain->images[i];
        view_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
        view_info.format = swapchain->image_format.format;
        view_info.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        view_info.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        view_info.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        view_info.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
        view_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        view_info.subresourceRange.baseMipLevel = 0;
        view_info.subresourceRange.levelCount = 1;
        view_info.subresourceRange.baseArrayLayer = 0;
        view_info.subresourceRange.layerCount = 1;

        vk_check(vkCreateImageView(vulkan_context.device.logical_device,
                                   &view_info, nullptr, &swapchain->views[i]));
    }

    ilog("swapchain created");
}

void destroy(VulkanSwapchain *swapchain) {}

void vulkan_swapchain_create(unsigned int width, unsigned int height,
                             VulkanSwapchain *out_swapchain) {
    create(width, height, out_swapchain);
}

void vulkan_swapchain_recreate(unsigned int width, unsigned int height,
                               VulkanSwapchain *swapchain) {
    destroy(swapchain);
    create(width, height, swapchain);
}

void vulkan_swapchain_destroy(VulkanSwapchain *swapchain) {
    destroy(swapchain);
}

bool vulkan_swapchain_acquire_next_image_index(
    VulkanSwapchain *swapchain, unsigned int timeout_ns,
    VkSemaphore image_available_semaphore, VkFence fence,
    unsigned int *out_image_index) {
    VkResult result = vkAcquireNextImageKHR(
        vulkan_context.device.logical_device, swapchain->handle, timeout_ns,
        image_available_semaphore, fence, out_image_index);

    if (result == VK_ERROR_OUT_OF_DATE_KHR) {
        vulkan_swapchain_recreate(vulkan_context.framebuffer_width,
                                  vulkan_context.framebuffer_height, swapchain);
        return false;
    } else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
        flog("failed to acquire next image index");
        return false;
    }
    return true;
}

void vulkan_swapchain_present(VulkanSwapchain *swapchain,
                              VkQueue grahpics_queue, VkQueue present_queue,
                              VkSemaphore render_complete_semaphore,
                              unsigned int present_image_index) {
    VkPresentInfoKHR present_info = {VK_STRUCTURE_TYPE_PRESENT_INFO_KHR};
    present_info.waitSemaphoreCount = 1;
    present_info.pWaitSemaphores = &render_complete_semaphore;
    present_info.swapchainCount = 1;
    present_info.pSwapchains = &swapchain->handle;
    present_info.pImageIndices = &present_image_index;

    VkResult result = vkQueuePresentKHR(present_queue, &present_info);
    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR) {
        vulkan_swapchain_recreate(vulkan_context.framebuffer_width,
                                  vulkan_context.framebuffer_height, swapchain);
    } else if (result != VK_SUCCESS) {
        flog("failed to present swap chain image");
    }
}