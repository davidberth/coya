#include <stdio.h>

typedef struct Context {
    unsigned int value_1; // holds first contextual data
    unsigned int value_2; // holds second contextual data
} Context;

typedef struct Event Event; // forward declaration of Event

typedef void (*EventReceiver)(
    Event *event); // pointer to an event handling function

struct Event {
    int event_type;         // holds the event type
    void *sender;           // holds the sender of the event
    Context context;        // holds contextual information
    EventReceiver receiver; // pointer to the handler function
};

// dispatches the event by calling its receiver if available
void dispatch_event(Event *event) {
    if (event->receiver) { // checks if the event has a receiver
        event->receiver(event);
    }
}

// sample receiver that handles an event
void sample_receiver(Event *event) {
    printf("received event type: %d\n",
           event->event_type); // prints the event type
}

int main(void) {
    Event sample_event;                      // creates a sample event instance
    sample_event.event_type = 1;             // sets the event type
    sample_event.sender = NULL;              // no sender provided
    sample_event.context.value_1 = 10;       // sets first contextual value
    sample_event.context.value_2 = 20;       // sets second contextual value
    sample_event.receiver = sample_receiver; // assigns the sample receiver

    dispatch_event(&sample_event); // dispatches the sample event

    return 0; // returns success
}