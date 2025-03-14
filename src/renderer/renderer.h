#pragma once
#include "core/event.h"
#include "math/types.h"
#include "resources/types.h"
#include "renderer/types.h"

typedef struct {
    mat4 projection;
    mat4 view;
    float near_clip;
    float far_clip;

    Texture default_texture;
} RendererGlobalState;

bool renderer_init();
void renderer_cleanup();
void renderer_render_frame();
void renderer_set_view(mat4 view);

void renderer_create_texture(const char *name, bool auto_release,
  unsigned int width, unsigned int height, unsigned short int channel_count,
  const unsigned char *pixels, bool has_alpha, Texture *out_texture);
void renderer_destroy_texture(Texture *texture);

bool renderer_begin_frame(float delta_time);
void renderer_update_global_state(mat4 projection, mat4 view,
  vec3 view_position, vec4 ambient_color, int mode, float delta_time);
bool renderer_end_frame(float delta_time);
void renderer_update_object(GeometryRenderData data);
void renderer_on_resize(EventContext event_context);