#include <pebble.h>
#include "window_with_timer.h"
#include "window_edit_number.h"

static void edit_number_callback(int key, int number) {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "callback: key=%d, number=%d", key, number);
}

enum {
    NUMBER_FIRST_SET = 0x5
};

int main(void) {
    show_window_edit_number(NUMBER_FIRST_SET, 12, edit_number_callback);

    app_event_loop();
}
