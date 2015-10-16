#ifdef __MINGW32__
#include "pebble.h/pebble.h"
#else
#include <pebble.h>
#endif
#include "ui/gym/window_menu.h"

int main(void) {
//    show_window_with_timer();
    show_window_menu();

    app_event_loop();

    return 0;
}
