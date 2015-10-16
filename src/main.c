#include <pebble.h>
#include "ui_gym/window_menu.h"

int main(void) {
//    show_window_with_timer();
    show_window_menu();

    app_event_loop();

    return 0;
}
