#include "core/event.h"
#include "core/logger.h"

const char *event_type_string[] = {
    "Keyboard",
    "Mouse Button",
    "Mouse Move",
};

// global event manager arrays to store event listeners for each event type
static Event event_listeners[EVENT_TYPE_COUNT][MAX_EVENTS] = {0};
// array of count of event listeners registered per event type
static unsigned int event_listener_counts[3] = {0};

// function to register an event handler for a given event type
void register_event_handler(EventType event_type, EventHandler handler) {
    // check for null handler
    if (!handler) {
        return;
    }
    // if there is space to register a new handler
    if (event_listener_counts[event_type] < MAX_EVENTS) {
        // add new event listener
        event_listeners[event_type][event_listener_counts[event_type]]
            .receiver = handler;
        // increment count for this event type
        event_listener_counts[event_type]++;

        ilog("Registered event handler for event type %s",
             event_type_string[event_type]);
    } else {
        elog("Cannot register event handler for event type %s. Maximum number "
             "of event listeners reached.",
             event_type_string[event_type]);
    }
}

// function to trigger an event for a given event type with a provided
// context
void trigger_event(EventType event_type, EventContext event_context) {
    unsigned int i;
    // iterate through all registered event listeners for this event type
    for (i = 0; i < event_listener_counts[event_type]; i++) {
        // check if event handler exists before calling
        if (event_listeners[event_type][i].receiver) {
            event_listeners[event_type][i].receiver(event_context);
        }
    }
}