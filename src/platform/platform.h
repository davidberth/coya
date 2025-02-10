#pragma once

bool platform_init(int width, int height, char *title);
void platform_poll_events();
bool platform_should_close();
void platform_cleanup();

char *platform_get_current_directory();