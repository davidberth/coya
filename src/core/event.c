#include "core/event.h"
#include "core/logger.h"

constexpr unsigned int max_events = 5;

const char *event_type_string[] = {"Key Down",   "Key Up",       "Mouse Button",
                                   "Mouse Move", "Mouse Scroll", "Resize",
                                   "Input Down", "Input Up",     "Input Zoom"};

// global event manager arrays to store event listeners for each event type
static EventHandler event_handler[EVENT_TYPE_COUNT][max_events] = {0};
// array of count of event listeners registered per event type
static unsigned int event_handler_counts[EVENT_TYPE_COUNT] = {0};

// function to register an event handler for a given event type
void register_event_handler(EventType event_type, EventHandler handler) {
    // check for nullptr handler
    if (!handler) {
        return;
    }
    // if there is space to register a new handler
    if (event_handler_counts[event_type] < max_events) {
        // add new event listener
        event_handler[event_type][event_handler_counts[event_type]] = handler;
        // increment count for this event type
        event_handler_counts[event_type]++;

        ilog("registered event handler for event type %s",
             event_type_string[event_type]);
    } else {
        flog("cannot register event handler for event type %s. Maximum number "
             "of event listeners reached.",
             event_type_string[event_type]);
    }
}

// function to trigger an event for a given event type with a provided
// context
void trigger_event(EventType event_type, EventContext event_context) {
    unsigned int i;
    // iterate through all registered event listeners for this event type
    for (i = 0; i < event_handler_counts[event_type]; i++) {
        // check if event handler exists before calling
        if (event_handler[event_type][i]) {
            event_handler[event_type][i](event_context);
        }
    }
}