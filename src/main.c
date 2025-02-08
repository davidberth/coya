#include "configure/constants.h"
#include "core/logger.h"
#include "platform/window.h"

int main() {
    init_logger();
    dlog("Starting application");
    char title[256];
    snprintf(title, 256, "%s %s %s", APPNAME, APPVERSION, RENDERER);
    Window window;
    if (!window_init(&window, 800, 600, title)) {
        elog("Window initialization failed");
        return 1;
    }

    while (!window.shouldClose) {
        window_poll_events(&window);
    }

    window_cleanup(&window);
    dlog("Application shutdown");
    return 0;
}
