#include <pebble.h>
#include "ui/gym/window_menu.h"
#include "ui/upload/window_upload.h"

int main(void) {
//    show_window_with_timer();
    show_window_menu();

    app_event_loop();
}
