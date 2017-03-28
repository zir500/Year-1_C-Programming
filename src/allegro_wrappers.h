#ifndef __ALLEGRO_WRAPPERS_H__
#define __ALLEGRO_WRAPPERS_H__

/* This header contains a few extra wrappers for allegro which weren't included in graphics_lib */

void flush_event_queue();
int event_mouse_button_up_and_coords(int* x, int* y);
int event_mouse_button_down_and_coords(int* x, int* y);

#endif // __ALLEGRO_WRAPPERS_H__
