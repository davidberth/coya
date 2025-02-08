#pragma once
#include <stdbool.h>
#include <windows.h>

typedef struct {
    HWND handle;
    HINSTANCE instance;
    int width;
    int height;
    const char* title;
    bool shouldClose;
} Window;

bool window_init(Window* window, int width, int height, char* title);
void window_poll_events(Window* window);
void window_cleanup(Window* window);
