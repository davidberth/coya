#include "configure/constants.h"
#include "core/event.h"
#include "core/logger.h"
#include "platform/platform.h"
#include <windows.h>
#include <windowsx.h>
#include <winuser.h>

HWND window_handle = nullptr;
HINSTANCE window_hinstance = nullptr;

LRESULT CALLBACK WindowProc(
  HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_CLOSE:
        DestroyWindow(hwnd);
        return 0;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    case WM_SIZE:
        EventContext context;
        context.uint[0] = LOWORD(lParam);
        context.uint[1] = HIWORD(lParam);
        trigger_event(EVENT_TYPE_RESIZE, context);
        return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

bool platform_init(int width, int height, char *title) {

    WNDCLASSEX wc = {0};
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = GetModuleHandle(nullptr);
    wc.hCursor = LoadCursor(nullptr, IDC_CROSS);
    wc.lpszClassName = APPNAME;

    HICON icon = (HICON)LoadImage(wc.hInstance, "resources/icons/main.ico",
      IMAGE_ICON, 32, 32, LR_LOADFROMFILE);

    if (!icon) {
        elog("failed to load icon");
        icon = LoadIcon(nullptr, IDI_APPLICATION);
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
      temp_rect.bottom - temp_rect.top;            // compute adjusted height
    int pos_x = (screen_width - window_width) / 2; // compute center x position
    int pos_y =
      (screen_height - window_height) / 2; // compute center y position

    window_handle =
      CreateWindowEx(0, APPNAME, title, WS_OVERLAPPEDWINDOW, pos_x, pos_y,
        window_width, window_height, nullptr, nullptr, wc.hInstance, nullptr);
    if (!window_handle) {
        elog("failed to create window");
        return false;
    }

    // set our global hinstance variable
    window_hinstance = wc.hInstance;
    ShowWindow(window_handle, SW_SHOW);

    return true;
}

bool platform_should_close() { return window_handle == nullptr; }

void platform_poll_events() {
    MSG msg;
    while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
        if (msg.message == WM_QUIT) {
            window_handle = nullptr;
            return;
        } else if (msg.message == WM_KEYDOWN) {
            // check if key down event is a repeat
            if (msg.lParam & (1 << 30))
                break;
            EventContext context;
            context.uint[0] = msg.wParam;

            trigger_event(EVENT_TYPE_KEY_DOWN, context);
            if (msg.wParam == VK_ESCAPE) {
                window_handle = nullptr;
                return;
            }
        } else if (msg.message == WM_KEYUP) {
            EventContext context;
            context.uint[0] = msg.wParam;
            trigger_event(EVENT_TYPE_KEY_UP, context);
        } else if (msg.message == WM_LBUTTONDOWN) {
            EventContext context;
            context.uint[0] = 0; // left mouse button
            trigger_event(EVENT_TYPE_MOUSE_BUTTON, context);
        } else if (msg.message == WM_RBUTTONDOWN) {
            EventContext context;
            context.uint[0] = 1; // right mouse button
            trigger_event(EVENT_TYPE_MOUSE_BUTTON, context);
        } else if (msg.message == WM_MOUSEMOVE) {
            EventContext context;
            context.sint[0] = GET_X_LPARAM(msg.lParam);
            context.sint[1] = GET_Y_LPARAM(msg.lParam);

            trigger_event(EVENT_TYPE_MOUSE_MOVE, context);
        } else if (msg.message == WM_MOUSEWHEEL) {
            EventContext context;
            context.sint[0] = GET_WHEEL_DELTA_WPARAM(msg.wParam);
            trigger_event(EVENT_TYPE_MOUSE_SCROLL, context);
        }

        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

void platform_cleanup() { DestroyWindow(window_handle); }
