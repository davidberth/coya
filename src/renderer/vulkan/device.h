#pragma once

bool vulkan_device_create();

void vulkan_device_query_swapchain_support();

void vulkan_device_cleanup();

bool vulkan_device_detect_depth_format();