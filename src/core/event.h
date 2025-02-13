#pragma once

// maximum number of events per type
#define MAX_EVENTS 5

// define event types
typedef enum EventType {
    EVENT_TYPE_KEY_DOWN,
    EVENT_TYPE_KEY_UP,
    EVENT_TYPE_MOUSE_BUTTON,
    EVENT_TYPE_MOUSE_MOVE,
    EVENT_TYPE_MOUSE_SCROLL,
    EVENT_TYPE_RESIZE,
    EVENT_TYPE_INPUT_DOWN,
    EVENT_TYPE_INPUT_UP,
    EVENT_TYPE_INPUT_ZOOM,
    EVENT_TYPE_COUNT // total event types
} EventType;

// define event context to hold event data
typedef struct EventContext {
    union {
        unsigned int uint[2];
        int sint[2];
        float flt[2];
    };
} EventContext;

// define event handler function pointer type
typedef void (*EventHandler)(EventContext);

// declare function to register an event handler for a given event type
void register_event_handler(EventType event_type, EventHandler handler);

// declare function to trigger an event for a given event type with a provided
// context
void trigger_event(EventType event_type, EventContext event_context);
