#include <pebble.h>
#include "enums.h"
#include "window_with_timer.h"
#include "window_menu.h"

int main(void) {
//    show_window_with_timer();
    show_window_menu();

    app_event_loop();
}
