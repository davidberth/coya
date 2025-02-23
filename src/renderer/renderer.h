#pragma once
#include "core/event.h"

bool renderer_init();
void renderer_cleanup();
void renderer_on_resize(EventContext event_context);
