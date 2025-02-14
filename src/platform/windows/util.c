#include <windows.h>

void get_vulkan_platform_extensions(const char **extensions,
                                    unsigned int *count) {
    *count = 0;
    extensions[(*count)++] = "VK_KHR_win32_surface";
}