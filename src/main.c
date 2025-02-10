#include "configure/constants.h"
#include "core/logger.h"
#include "core/memory.h"
#include "platform/platform.h"

int main() {
    init_logger();
    dlog("starting application");
    ilog("application name: %s", APPNAME);
    wlog("application version: %s", APPVERSION);
    elog("renderer: %s", RENDERER);
    wlog("platform: %s", PLATFORM);
    ilog("date: %s", BUILDDATE);
    const int title_size = sizeof(APPNAME) + sizeof(APPVERSION) +
                           sizeof(RENDERER) + sizeof(PLATFORM) +
                           sizeof(BUILDDATE) + 9;
    char title[title_size];
    snprintf(title, title_size, "%s %s [%s] %s %s: %s", APPNAME, APPVERSION,
             BUILDDATE, RENDERER, PLATFORM, BUILDTYPE);

    if (!platform_init(1200, 800, title)) {
        elog("window initialization failed");
        return 1;
    }

    unsigned char *ptr = (unsigned char *)oalloc(1024, MEMORY_CATEGORY_GAME);
    if (ptr) {
        ilog("allocated memory for game: %zu bytes",
             get_allocated_memory(MEMORY_CATEGORY_GAME));

        ilog("total memory allocated: %zu bytes", get_total_allocated_memory());
        ofree(ptr);
        ilog("allocated memory for game after free: %zu bytes",
             get_allocated_memory(MEMORY_CATEGORY_GAME));
    } else {
        printf("failed to allocate memory\n");
    }

    while (!platform_should_close()) {
        platform_poll_events();
    }

    platform_cleanup();
    dlog("application shutdown");
    return 0;
}
