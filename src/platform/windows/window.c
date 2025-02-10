#include "configure/constants.h"
#include "core/logger.h"
#include "platform/platform.h"
#include <windows.h>

HWND handle = NULL;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam,
                            LPARAM lParam) {
    switch (uMsg) {
    case WM_CLOSE:
        DestroyWindow(hwnd);
        return 0;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

bool platform_init(int width, int height, char *title) {
    WNDCLASSEX wc = {0};
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = GetModuleHandle(NULL);
    wc.hCursor = LoadCursor(NULL, IDC_CROSS);
    wc.lpszClassName = APPNAME;

    HICON icon = (HICON)LoadImage(wc.hInstance, "resources/icons/main.ico",
                                  IMAGE_ICON, 32, 32, LR_LOADFROMFILE);

    if (!icon) {
        elog("failed to load icon");
        icon = LoadIcon(NULL, IDI_APPLICATION);
    }

    wc.hIcon = icon;

    if (!RegisterClassEx(&wc)) {
        elog("Failed to register window class");
        return false;
    }

    int screen_width = GetSystemMetrics(SM_CXSCREEN);  // get monitor width
    int screen_height = GetSystemMetrics(SM_CYSCREEN); // get monitor height
    RECT temp_rect = {0, 0, width, height};            // define window rect
    AdjustWindowRect(&temp_rect, WS_OVERLAPPEDWINDOW,
                     FALSE); // adjust window size for style
    int window_width =
        temp_rect.right - temp_rect.left; // compute adjusted width
    int window_height =
        temp_rect.bottom - temp_rect.top;          // compute adjusted height
    int pos_x = (screen_width - window_width) / 2; // compute center x position
    int pos_y =
        (screen_height - window_height) / 2; // compute center y position

    handle = CreateWindowEx(0, APPNAME, title, WS_OVERLAPPEDWINDOW, pos_x,
                            pos_y, window_width, window_height, NULL, NULL,
                            wc.hInstance, NULL);
    if (!handle) {
        elog("failed to create window");
        return false;
    }

    ShowWindow(handle, SW_SHOW);
    return true;
}

bool platform_should_close() { return handle == NULL; }

void platform_poll_events() {
    MSG msg;
    while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
        if (msg.message == WM_QUIT) {
            handle = NULL;
            return;
        }
        if (msg.message == WM_KEYDOWN) {
            if (msg.wParam == VK_ESCAPE) {
                handle = NULL;
                return;
            }
        }
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

void platform_cleanup() { DestroyWindow(handle); }

char *platform_get_current_directory() {
    static char path[MAX_PATH];
    GetCurrentDirectory(MAX_PATH, path);
    return path;
}
