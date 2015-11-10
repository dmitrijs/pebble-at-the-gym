#include <pebble.h>
#include "ui_gym/window_menu.h"
#include "ui_upload/window_upload.h"

int main(void) {
//    show_window_with_timer();
    show_window_menu();
//    show_window_upload();

    app_event_loop();

    return 0;
}
