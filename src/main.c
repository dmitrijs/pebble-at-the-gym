#include <pebble.h>
#include "enums.h"
#include "window_with_timer.h"
#include "window_edit_number.h"

int main(void) {
    show_window_with_timer();

    app_event_loop();
}
