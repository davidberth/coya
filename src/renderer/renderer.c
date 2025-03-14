#include "renderer/renderer.h"
#include "core/logger.h"
#include "platform/platform.h"
#include "math/omath.h"

RendererGlobalState renderer_state;

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
        angle += 0.0005f;
        if (angle > O_2PI) {
            angle -= O_2PI;
        }
        mat4 model = mat4_rotation_z(angle);

        GeometryRenderData data = {};
        data.object_id = 0;
        data.model = model;

        renderer_update_object(data);
        renderer_end_frame(delta_time);

        // sleep for a short time to avoid excessive CPU and GPU usage
        // platform_sleep(0.01);
    }
}

void renderer_set_view(mat4 view) { renderer_state.view = view; }