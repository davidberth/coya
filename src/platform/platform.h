#pragma once

bool platform_init(int width, int height, char *title);
void platform_poll_events();
bool platform_should_close();
void platform_cleanup();
void platform_get_window_size(unsigned int *width, unsigned int *height);

char *platform_set_to_root_directory();
