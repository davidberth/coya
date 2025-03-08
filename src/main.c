#include "configure/constants.h"
#include "core/event.h"
#include "core/logger.h"
#include "core/memory.h"
#include "input/input.h"
#include "platform/platform.h"
#include "renderer/renderer.h"
#include <stdio.h>
#include "math/omath.h"
#include "math/types.h"

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

    float aspect_ratio = 1280.0f / 720.0f;

    mat4 projection =
      mat4_perspective(O_DEG2RAD * 45.0f, aspect_ratio, 0.1f, 100.0f);

    // Position camera at (0,0,-3) looking at origin (0,0,0)
    vec3 eye = vec3_create(0.0f, 0.0f, 3.0f);
    vec3 center = vec3_zero();
    vec3 up = vec3_create(0.0f, 1.0f, 0.0f);
    mat4 view = mat4_look_at(eye, center, up);

    while (!platform_should_close()) {

        platform_poll_events();
        if (platform_is_window_showing()) {

            static double previous_time = 0.0;
            static int frame_count = 0;
            static double elapsed_time = 0.0;

            double current_time = platform_get_time();
            double delta_time = current_time - previous_time;
            previous_time = current_time;

            frame_count++;
            elapsed_time += delta_time;

            if (elapsed_time >= 2.0) {
                double framerate = frame_count / elapsed_time;
                dlog("framerate: %.2f fps", framerate);
                frame_count = 0;
                elapsed_time = 0.0;
            }
            if (renderer_begin_frame(0.0f)) {

                renderer_update_global_state(
                  projection, view, vec3_zero(), vec4_one(), 0);
                renderer_end_frame(0.0f);
            }
        }
    }
    renderer_cleanup();
    platform_cleanup();
    memory_cleanup();
    ilog("application shutdown");
    return 0;
}
