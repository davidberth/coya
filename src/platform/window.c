#include "window.h"
#include "configure/constants.h"
#include "core/logger.h"

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

bool window_init(Window *window, int width, int height, char *title) {
    window->instance = GetModuleHandle(NULL);
    window->width = width;
    window->height = height;

    window->title = title;
    window->shouldClose = false;

    WNDCLASSEX wc = {0};
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = window->instance;
    wc.hCursor = LoadCursor(NULL, IDC_CROSS);
    wc.lpszClassName = APPNAME;

    if (!RegisterClassEx(&wc)) {
        elog("Failed to register window class");
        return false;
    }

    RECT rect = {0, 0, width, height};
    AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

    window->handle = CreateWindowEx(
        0, APPNAME, title, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
        rect.right - rect.left, rect.bottom - rect.top, NULL, NULL,
        window->instance, NULL);

    if (!window->handle) {
        elog("Failed to create window");
        return false;
    }

    ShowWindow(window->handle, SW_SHOW);
    return true;
}

void window_poll_events(Window *window) {
    MSG msg;
    while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
        if (msg.message == WM_QUIT) {
            window->shouldClose = true;
            return;
        }
        if (msg.message == WM_KEYDOWN) {
            if (msg.wParam == VK_ESCAPE) {
                window->shouldClose = true;
                return;
            }
        }
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

void window_cleanup(Window *window) { DestroyWindow(window->handle); }
