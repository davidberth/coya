#include "input/input.h"
#include "core/event.h"
#include "core/logger.h"

// global array to store key states
static bool keys[256] = {0};
// defines if each key is mapped to a game input
static bool input_keys[256] = {0};

bool get_key_state(KeyMap key) { return keys[key]; }

void input_on_keydown(EventContext context) {
    keys[context.uint[0]] = true;
    ilog("key down: %d", context.uint[0]);
    // check if the key is mapped to a game input
    // if so forward the event to the game input system
    if (input_keys[context.uint[0]]) {
        trigger_event(EVENT_TYPE_INPUT_DOWN, context);
    }
}
void input_on_keyup(EventContext context) {
    keys[context.uint[0]] = false;

    // check if the key is mapped to a game input
    if (input_keys[context.uint[0]]) {
        trigger_event(EVENT_TYPE_INPUT_UP, context);
    }
}

void input_on_mouse_scroll(EventContext context) {
    ilog("mouse scroll: %d", context.sint[0]);
    trigger_event(EVENT_TYPE_INPUT_ZOOM, context);
}

void input_init() {
    register_event_handler(EVENT_TYPE_KEY_DOWN, input_on_keydown);
    register_event_handler(EVENT_TYPE_KEY_UP, input_on_keyup);
    register_event_handler(EVENT_TYPE_MOUSE_SCROLL, input_on_mouse_scroll);
    // TODO: possibly find a better way to iterate through all possible input
    // keys (not critical)
    input_keys[INPUT_KEY_UP] = true;
    input_keys[INPUT_KEY_DOWN] = true;
    input_keys[INPUT_KEY_LEFT] = true;
    input_keys[INPUT_KEY_RIGHT] = true;
    input_keys[INPUT_KEY_UP_LEFT] = true;
    input_keys[INPUT_KEY_UP_RIGHT] = true;
    input_keys[INPUT_KEY_DOWN_LEFT] = true;
    input_keys[INPUT_KEY_DOWN_RIGHT] = true;
    input_keys[INPUT_KEY_CENTER] = true;
}