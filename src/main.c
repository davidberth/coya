#include "configure/constants.h"
#include "core/logger.h"
#include "core/memory.h"
#include "platform/platform.h"

char *get_window_title() {
#define TITLE_SIZE                                                             \
    (sizeof(APPNAME) + sizeof(APPVERSION) + sizeof(RENDERER) +                 \
     sizeof(PLATFORM) + sizeof(BUILDDATE) + 9)

    static char title[TITLE_SIZE];
    snprintf(title, TITLE_SIZE, "%s %s [%s] %s %s: %s", APPNAME, APPVERSION,
             BUILDDATE, RENDERER, PLATFORM, BUILDTYPE);
    return title;
}

int main() {
    init_logger();
    dlog("starting application");
    ilog("application name: %s", APPNAME);
    wlog("application version: %s", APPVERSION);
    elog("renderer: %s", RENDERER);
    wlog("platform: %s", PLATFORM);
    ilog("date: %s", BUILDDATE);

    if (!platform_init(1200, 800, get_window_title())) {
        elog("window initialization failed");
        return 1;
    }

    while (!platform_should_close()) {
        platform_poll_events();
    }

    platform_cleanup();
    dlog("application shutdown");
    return 0;
}
