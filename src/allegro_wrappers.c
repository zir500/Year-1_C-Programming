#include "allegro_wrappers.h"
#include <graphics_lib.h>

/* Clears the event queue that graphics_lib uses.*/
void flush_event_queue() {
    /* event_queue is declared in graphics_lib.h */
    al_flush_event_queue(event_queue);
}

/* Checks whether the last event was the mouse button being released and the coords of the cursor */
int event_mouse_button_up_and_coords(int* x, int* y) {
    if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP) {
        *x = event.mouse.x;
        *y = event.mouse.y;
        return 1;
    } else {
        return 0;
    }
}

/* Checks whether the last event was the mouse button being pressed and the coords of the cursor */
int event_mouse_button_down_and_coords(int* x, int* y) {
    if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
        *x = event.mouse.x;
        *y = event.mouse.y;
        return 1;
    } else {
        return 0;
    }
}
