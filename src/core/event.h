#pragma once

// define maximum number of events per type
#define MAX_EVENTS 10

// define event types
typedef enum EventType {
    EVENT_TYPE_KEYBOARD,
    EVENT_TYPE_MOUSE_BUTTON,
    EVENT_TYPE_MOUSE_MOVE,
    EVENT_TYPE_COUNT // total event types
} EventType;

// define event context to hold event data
typedef struct EventContext {
    union {
        unsigned int uint[2];
        float flt[2];
    };
} EventContext;

// define event handler function pointer type
typedef void (*EventHandler)(EventContext);

// define the Event structure
typedef struct Event {
    EventContext context;  // event context for this event
    EventHandler receiver; // event handler for this event
} Event;

// declare the example event handler
void example_handler(EventContext event_context);

// declare function to dispatch an event
void dispatch_event(Event event);

// declare function to register an event handler for a given event type
void register_event_handler(EventType event_type, EventHandler handler);

// declare function to trigger an event for a given event type with a provided
// context
void trigger_event(EventType event_type, EventContext event_context);
