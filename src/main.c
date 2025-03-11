#include "configure/constants.h"
#include "core/event.h"
#include "core/logger.h"
#include "core/memory.h"
#include "input/input.h"
#include "platform/platform.h"
#include "renderer/renderer.h"
#include "camera.h"
#include <stdio.h>

typedef struct {
    mat4 view;
    Camera camera;
} GameState;

GameState game_state;

char *get_window_title() {
#define TITLE_SIZE                                                             \
    (sizeof(APPNAME) + sizeof(APPVERSION) + sizeof(RENDERER) +                 \
      sizeof(PLATFORM) + sizeof(BUILDDATE) + 11)

    static char title[TITLE_SIZE];
    snprintf(title, TITLE_SIZE, "%s %s [%s] %s %s: %s", APPNAME, APPVERSION,
      BUILDDATE, RENDERER, PLATFORM, BUILDTYPE);
    return title;
}

void on_mouse_button(EventContext context) {
    ilog("mouse button: %d", context.uint[0]);
}

void on_mouse_move(EventContext context) {
    ilog("mouse move: %d, %d", context.sint[0], context.sint[1]);
}

void on_input_down(EventContext context) {
    ilog("input down: %d", context.uint[0]);
}

void on_input_up(EventContext context) {
    ilog("input up: %d", context.uint[0]);
}

int main() {
    init_logger();
    ilog("starting application");
    ilog("application name: %s", APPNAME);
    ilog("application version: %s", APPVERSION);
    ilog("renderer: %s", RENDERER);
    ilog("platform: %s", PLATFORM);
    ilog("date: %s", BUILDDATE);
    char *root_directory = platform_set_to_root_directory();
    ilog("current directory: %s", root_directory);

    if (!platform_init(1000, 600, get_window_title())) {
        elog("window initialization failed");
        return 1;
    }

    register_event_handler(EVENT_TYPE_MOUSE_BUTTON, on_mouse_button);
    register_event_handler(EVENT_TYPE_INPUT_DOWN, on_input_down);
    register_event_handler(EVENT_TYPE_INPUT_UP, on_input_up);

    input_init();

    if (!renderer_init()) {
        return 1;
    }

    // initialize camera
    camera_init(&game_state.camera);

    while (!platform_should_close()) {
        platform_poll_events();

        // update camera and get the view matrix
        camera_update(&game_state.camera);
        game_state.view = camera_get_view_matrix(&game_state.camera);

        if (platform_is_window_showing()) {
            renderer_set_view(game_state.view);
            renderer_render_frame();
        }
    }
    renderer_cleanup();
    platform_cleanup();
    memory_cleanup();
    ilog("application shutdown");
    return 0;
}
