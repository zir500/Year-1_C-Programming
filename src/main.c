#include <stdio.h>
#include <string.h>
#include <math.h>
#include <graphics_lib.h>

#include "constants.h"
#include "displays.h"
#include "processes.h"



int main(){
    initwindow(WINDOW_WIDTH, WINDOW_HEIGHT);
    initfont();
    initmouse();

    create_event_queue();
    reg_mouse_events();
    reg_display_events();

    menuScreen();

    finish();
    return 0;
}
