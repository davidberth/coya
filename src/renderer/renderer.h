#pragma once
#include "core/event.h"

bool renderer_init();
void renderer_cleanup();

bool renderer_begin_frame(float delta_time);
bool renderer_end_frame(float delta_time);

void renderer_on_resize(EventContext event_context);
