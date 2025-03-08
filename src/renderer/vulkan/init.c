#include "core/logger.h"
#include "core/memory.h"
#include "core/event.h"
#include "renderer/renderer.h"
#include "platform/platform.h"
#include "renderer/vulkan/shader/shader.h"
#include "platform/util.h"
#include "types.h"
#include "util.h"
#include "device.h"
#include "swapchain.h"
#include "renderpass.h"
#include "command_buffer.h"
#include "framebuffer.h"
#include "fence.h"
#include "math/types.h"
#include "buffer.h"

// shader
#include "shader/shader.h"
#include <vulkan/vulkan_core.h>

extern VulkanContext vulkan_context;

constexpr unsigned int max_extensions = 6;

VKAPI_ATTR VkBool32 VKAPI_CALL vk_debug_callback(
  VkDebugUtilsMessageSeverityFlagBitsEXT message_severity,
  VkDebugUtilsMessageTypeFlagsEXT message_type,
  const VkDebugUtilsMessengerCallbackDataEXT *callback_data, void *user_data) {

    if (message_severity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT) {
        elog("validation layer: %s", callback_data->pMessage);
    } else if (message_severity >=
               VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) {
        wlog("validation layer: %s", callback_data->pMessage);
    } else {
        dlog("validation layer: %s", callback_data->pMessage);
    }

    return VK_FALSE;
}

void upload_data_range(VkCommandPool pool, VkFence fence, VkQueue queue,
  VulkanBuffer *buffer, unsigned int offset, unsigned int size, void *data) {
    VkBufferUsageFlags flags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                               VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
    VulkanBuffer staging;
    vulkan_buffer_create(
      size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, flags, true, &staging);

    vulkan_buffer_load_data(&staging, 0, size, 0, data);
    vulkan_buffer_copy_to(
      pool, fence, queue, staging.handle, 0, buffer->handle, offset, size);

    vulkan_buffer_destroy(&staging);
}

int find_memory_index(unsigned int type_filter, unsigned int property_flags) {
    VkPhysicalDeviceMemoryProperties memory_properties;
    vkGetPhysicalDeviceMemoryProperties(
      vulkan_context.device.physical_device, &memory_properties);

    for (unsigned int i = 0; i < memory_properties.memoryTypeCount; i++) {
        if ((type_filter & (1 << i)) &&
            (memory_properties.memoryTypes[i].propertyFlags & property_flags) ==
              property_flags) {
            return i;
        }
    }
    wlog("unable to find suitable memory type");

    return -1;
}

bool create_buffers() {
    ilog("creating vertex and index buffers");
    VkMemoryPropertyFlagBits memory_prop_flags =
      VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;

    constexpr unsigned int vertex_buffer_size = sizeof(Vertex3D) * 100;
    if (!vulkan_buffer_create(vertex_buffer_size,
          VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT |
            VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
          memory_prop_flags, true, &vulkan_context.main_vertex_buffer)) {
        elog("failed to create vertex buffer");
        return false;
    }
    vulkan_context.geometry_vertex_offset = 0;

    constexpr unsigned int index_buffer_size = sizeof(uint32_t) * 100;
    if (!vulkan_buffer_create(index_buffer_size,
          VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT |
            VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
          memory_prop_flags, true, &vulkan_context.main_index_buffer)) {
        elog("failed to create index buffer");
        return false;
    }
    vulkan_context.geometry_index_offset = 0;

    return true;
}

void create_command_buffers() {
    if (!vulkan_context.graphics_command_buffers) {
        vulkan_context.graphics_command_buffers = (VulkanCommandBuffer *)oalloc(
          sizeof(VulkanCommandBuffer) * vulkan_context.swapchain.image_count,
          MEMORY_CATEGORY_VULKAN);

        for (unsigned int i = 0; i < vulkan_context.swapchain.image_count;
          i++) {
            vulkan_context.graphics_command_buffers[i].handle = 0;
            vulkan_context.graphics_command_buffers[i].state =
              COMMAND_BUFFER_STATE_NOT_ALLOCATED;
        }
    }

    for (unsigned int i = 0; i < vulkan_context.swapchain.image_count; i++) {
        if (vulkan_context.graphics_command_buffers[i].handle) {
            vulkan_command_buffer_free(
              vulkan_context.device.graphics_command_pool,
              &vulkan_context.graphics_command_buffers[i]);
        }

        vulkan_command_buffer_allocate(
          vulkan_context.device.graphics_command_pool, true,
          &vulkan_context.graphics_command_buffers[i]);
    }
}

void regenerate_framebuffers(
  VulkanSwapchain *swapchain, VulkanRenderpass *renderpass) {
    ilog("regenerating framebuffers");
    for (unsigned int i = 0; i < swapchain->image_count; i++) {
        unsigned int attachment_count = 2;
        VkImageView attachments[2] = {
          swapchain->views[i], vulkan_context.swapchain.depth_attachment.view};

        vulkan_framebuffer_create(renderpass, vulkan_context.framebuffer_width,
          vulkan_context.framebuffer_height, attachment_count, attachments,
          &vulkan_context.swapchain.framebuffers[i]);
    }
}

bool renderer_init() {
    vulkan_context.find_memory_index = find_memory_index;
    vulkan_context.allocator = nullptr;
    vulkan_context.graphics_command_buffers = nullptr;
    vulkan_context.swapchain.framebuffers = nullptr;
    vulkan_context.framebuffer_resize_needed = false;
    vulkan_context.device.swapchain_support.formats = nullptr;
    vulkan_context.device.swapchain_support.present_modes = nullptr;

    register_event_handler(EVENT_TYPE_RESIZE, renderer_on_resize);

    // grab the window width and height
    platform_get_window_size(
      &vulkan_context.framebuffer_width, &vulkan_context.framebuffer_height);

    VkApplicationInfo app_info = {VK_STRUCTURE_TYPE_APPLICATION_INFO};
    app_info.pApplicationName = "Coya";
    app_info.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    app_info.pEngineName = "No Engine";
    app_info.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    app_info.apiVersion = VK_API_VERSION_1_4;

    VkInstanceCreateInfo create_info = {VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO};
    create_info.pApplicationInfo = &app_info;

    const char *extensions[max_extensions];
    unsigned int extension_count = 0;
    get_vulkan_platform_extensions(extensions, &extension_count);
    extensions[extension_count++] = VK_KHR_SURFACE_EXTENSION_NAME;
#if defined(_DEBUG)
    extensions[extension_count++] = VK_EXT_DEBUG_UTILS_EXTENSION_NAME;
#endif

    ilog("vulkan extensions:");
    for (unsigned int i = 0; i < extension_count; i++) {
        ilog(" extension: %s", extensions[i]);
    }

    create_info.enabledExtensionCount = extension_count;
    create_info.ppEnabledExtensionNames = extensions;

#if defined(_DEBUG)
    ilog("validation layers enabled");

    const char *validation_layers[1];
    validation_layers[0] = "VK_LAYER_KHRONOS_validation";

    create_info.enabledLayerCount = 1;
    create_info.ppEnabledLayerNames = validation_layers;
#else
    ilog("validation layers disabled");
    create_info.enabledLayerCount = 0;
    create_info.ppEnabledLayerNames = nullptr;
#endif

    vk_check(vkCreateInstance(
      &create_info, vulkan_context.allocator, &vulkan_context.instance));
    ilog("vulkan instance created");

#if defined(_DEBUG)
    unsigned int log_serverity =
      VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT |
      VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT;

    VkDebugUtilsMessengerCreateInfoEXT debug_create_info = {
      VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT};
    debug_create_info.messageSeverity = log_serverity;
    debug_create_info.messageType =
      VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
      VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
      VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    debug_create_info.pfnUserCallback = vk_debug_callback;

    PFN_vkCreateDebugUtilsMessengerEXT func =
      (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
        vulkan_context.instance, "vkCreateDebugUtilsMessengerEXT");
    vk_check(func(vulkan_context.instance, &debug_create_info,
      vulkan_context.allocator, &vulkan_context.debug_messenger));
    dlog("vulkan debug messenger created");

#endif

    ilog("creating vulkan surface");
    if (!create_vulkan_surface()) {
        elog("failed to create vulkan surface");
        return false;
    }

    if (!vulkan_device_create()) {
        elog("failed to create vulkan device");
        return false;
    }

    vulkan_swapchain_create(vulkan_context.framebuffer_width,
      vulkan_context.framebuffer_height, &vulkan_context.swapchain);

    ilog("creating renderpass");
    vulkan_renderpass_create(&vulkan_context.main_renderpass, 0.0f, 0.0f,
      vulkan_context.framebuffer_width, vulkan_context.framebuffer_height, 0.0f,
      0.0f, 0.2f, 1.0f, 1.0f, 0);

    // swapchan framebuffers
    vulkan_context.swapchain.framebuffers = (VulkanFramebuffer *)oalloc(
      sizeof(VulkanFramebuffer) * vulkan_context.swapchain.image_count,
      MEMORY_CATEGORY_VULKAN);

    regenerate_framebuffers(
      &vulkan_context.swapchain, &vulkan_context.main_renderpass);

    create_command_buffers();

    vulkan_context.image_available_semaphores = (VkSemaphore *)oalloc(
      sizeof(VkSemaphore) * vulkan_context.swapchain.max_frames_in_flight,
      MEMORY_CATEGORY_VULKAN);

    vulkan_context.queue_complete_semaphores = (VkSemaphore *)oalloc(
      sizeof(VkSemaphore) * vulkan_context.swapchain.max_frames_in_flight,
      MEMORY_CATEGORY_VULKAN);

    vulkan_context.in_flight_fences = (VulkanFence *)oalloc(
      sizeof(VulkanFence) * vulkan_context.swapchain.max_frames_in_flight,
      MEMORY_CATEGORY_VULKAN);

    for (unsigned int i = 0; i < vulkan_context.swapchain.max_frames_in_flight;
      ++i) {
        VkSemaphoreCreateInfo semaphore_create_info = {
          VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO};
        vk_check(vkCreateSemaphore(vulkan_context.device.logical_device,
          &semaphore_create_info, vulkan_context.allocator,
          &vulkan_context.image_available_semaphores[i]));
        vk_check(vkCreateSemaphore(vulkan_context.device.logical_device,
          &semaphore_create_info, vulkan_context.allocator,
          &vulkan_context.queue_complete_semaphores[i]));

        // we start in a signaled state so we can render the first frame
        vulkan_fence_create(true, &vulkan_context.in_flight_fences[i]);
    }

    vulkan_context.images_in_flight = (VulkanFence **)oalloc(
      sizeof(VulkanFence *) * vulkan_context.swapchain.image_count,
      MEMORY_CATEGORY_VULKAN);
    for (unsigned int i = 0; i < vulkan_context.swapchain.image_count; i++) {
        vulkan_context.images_in_flight[i] = nullptr;
    }

    if (!shader_create(&vulkan_context.main_shader)) {
        elog("failed to create main shader");
        return false;
    }

    if (!create_buffers()) {
        elog("failed to create buffers");
        return false;
    }

    // TODO: temporary test code
    constexpr float f = 1.0f;
    constexpr unsigned int vert_count = 4;
    Vertex3D verts[vert_count];

    verts[0].position.x = -0.25f * f;
    verts[0].position.y = 0.25f * f;
    verts[0].position.z = 0.0f * f;

    verts[1].position.x = 0.25f * f;
    verts[1].position.y = -0.25f * f;
    verts[1].position.z = 0.0f * f;

    verts[2].position.x = -0.25f * f;
    verts[2].position.y = -0.25f * f;
    verts[2].position.z = 0.0f * f;

    verts[3].position.x = 0.25f * f;
    verts[3].position.y = 0.25f * f;
    verts[3].position.z = 0.0f * f;

    constexpr unsigned int index_count = 6;
    unsigned int indices[index_count] = {0, 1, 2, 0, 3, 1};

    upload_data_range(vulkan_context.device.graphics_command_pool, 0,
      vulkan_context.device.graphics_queue, &vulkan_context.main_vertex_buffer,
      0, sizeof(Vertex3D) * vert_count, verts);

    upload_data_range(vulkan_context.device.graphics_command_pool, 0,
      vulkan_context.device.graphics_queue, &vulkan_context.main_index_buffer,
      0, sizeof(uint32_t) * index_count, indices);

    // TODO: this is temporary code

    ilog("vulkan renderer initialized");
    return true;
}
