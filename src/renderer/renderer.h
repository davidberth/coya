#pragma once
#include "core/event.h"
#include "math/types.h"

bool renderer_init();
void renderer_cleanup();

bool renderer_begin_frame(float delta_time);
void renderer_update_global_state(
  mat4 projection, mat4 view, vec3 view_position, vec4 ambient_color, int mode);
bool renderer_end_frame(float delta_time);
void renderer_update_object(mat4 model);

void renderer_on_resize(EventContext event_context);
