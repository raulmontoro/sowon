/**************** EVENTS */

typedef enum Event {
    NONE,
    SPACE,
    EQUALS,
    MINUS,
    ZERO,
    SHIFTZERO,
    F5,
    F11,
    WHEELUP,
    WHEELDOWN
} Event;

void eventLoop(int *quit, Event *event);

