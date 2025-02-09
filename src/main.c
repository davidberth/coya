#include "configure/constants.h"
#include "core/logger.h"
#include "platform/window.h"

int main() {
    init_logger();
    dlog("starting application");
    ilog("application name: %s", APPNAME);
    wlog("application version: %s", APPVERSION);
    elog("renderer: %s", RENDERER);
    wlog("platform: %s", PLATFORM);

    const int title_size = sizeof(APPNAME) + sizeof(APPVERSION) +
                           sizeof(RENDERER) + sizeof(PLATFORM) + 4;
    char title[title_size];
    snprintf(title, title_size, "%s %s %s %s", APPNAME, APPVERSION, RENDERER,
             PLATFORM);
    Window window;
    if (!window_init(&window, 800, 600, title)) {
        elog("window initialization failed");
        return 1;
    }

    while (!window.shouldClose) {
        window_poll_events(&window);
    }

    window_cleanup(&window);
    dlog("application shutdown");
    return 0;
}
