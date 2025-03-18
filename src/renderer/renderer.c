#include "renderer/renderer.h"
#include "core/logger.h"
#include "platform/platform.h"
#include "math/omath.h"
#include "core/event.h"
#include "util/ppm.h"
#include "input/input.h"
#include <string.h>
#include <stdio.h>

RendererGlobalState renderer_state;

void enable_vsync(bool vsync) { renderer_state.vsync = vsync; }

bool load_texture(const char *texture_name, Texture *t) {
    char *format_str = "resources/textures/%s.%s";
    char full_file_path[256];
    unsigned int required_channel_count = 4;

    snprintf(full_file_path, 256, format_str, texture_name, "ppm");

    Texture temp;
    PPM *ppm = load_ppm(full_file_path);

    temp.width = ppm->width;
    temp.height = ppm->height;
    temp.channel_count = 4;

    if (ppm->pixels) {
        unsigned int current_generation = t->generation;
        t->generation = INVALID_ID;

        unsigned int total_size =
          temp.width * temp.height * required_channel_count;

        renderer_create_texture(texture_name, false, temp.width, temp.height,
          required_channel_count, ppm->pixels, total_size, &temp);

        Texture old = *t;
        *t = temp;

        renderer_destroy_texture(&old);

        if (current_generation == INVALID_ID) {
            t->generation = 0;
        } else {
            t->generation = current_generation + 1;
        }
        free_ppm(ppm);
        return true;
    }
    elog("failed to load texture: %s", texture_name);
    return false;
}

void renderer_on_input(EventContext context) {
    if (context.uint[0] == INPUT_KEY_T) {
        static unsigned int current_texture = 2;
        dlog("loading new texture: %d", context.uint[0]);

        const char *name[3] = {"tex1", "tex2", "tex3"};
        current_texture = (current_texture + 1) % 3;

        load_texture(name[current_texture], &renderer_state.test_diffuse);
    } else if (context.uint[0] == INPUT_KEY_V) {

        enable_vsync(!renderer_state.vsync);
        dlog("vsync: %s", renderer_state.vsync ? "enabled" : "disabled");

        unsigned int width;
        unsigned int height;
        platform_get_window_size(&width, &height);
        // trigger a resize to force a swapchain recreation
        EventContext context;
        context.uint[0] = width;
        context.uint[1] = height;
        trigger_event(EVENT_TYPE_RESIZE, context);
    }
}

bool renderer_init() {
    unsigned int width;
    unsigned int height;
    platform_get_window_size(&width, &height);
    float aspect_ratio = (float)width / (float)height;
    renderer_state.vsync = true;
    renderer_state.near_clip = 0.1f;
    renderer_state.far_clip = 100.0f;
    renderer_state.projection = mat4_perspective(O_DEG2RAD * 45.0f,
      aspect_ratio, renderer_state.near_clip, renderer_state.far_clip);

    // Position camera at (0,0,-3) looking at origin (0,0,0)
    vec3 eye = vec3_create(0.0f, 0.0f, 3.0f);
    vec3 center = vec3_zero();
    vec3 up = vec3_create(0.0f, 1.0f, 0.0f);
    renderer_state.view = mat4_look_at(eye, center, up);

    bool vulkan_init_result = renderer_init_vulkan();
    register_event_handler(EVENT_TYPE_INPUT_DOWN, renderer_on_input);
    return vulkan_init_result;
}

void renderer_render_frame() {
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
        dlog("framerate: %.2f FPS", framerate);
        frame_count = 0;
        elapsed_time = 0.0;
    }
    if (renderer_begin_frame(delta_time)) {

        renderer_update_global_state(renderer_state.projection,
          renderer_state.view, vec3_zero(), vec4_one(), 0, delta_time);

        static float angle = 0.0f;
        angle += delta_time;
        if (angle > O_2PI) {
            angle -= O_2PI;
        }
        mat4 model = mat4_rotation_z(angle);

        GeometryRenderData data = {};
        data.object_id = 0;
        data.model = model;
        data.textures[0] = &renderer_state.test_diffuse;

        renderer_update_object(data);
        renderer_end_frame(delta_time);

        // sleep for a short time to avoid excessive CPU and GPU usage
        // platform_sleep(0.001);
    }
}

void create_texture(Texture *t) {
    memset(t, 0, sizeof(Texture));
    t->generation = INVALID_ID;
}

void renderer_set_view(mat4 view) { renderer_state.view = view; }
