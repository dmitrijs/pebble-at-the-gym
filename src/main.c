#include <pebble.h>
#include "ui_gym/window_menu.h"
#include "ui/window_error.h"
#include "tests/tests.h"

int main(void) {
    if (tests_pass()) {
        show_window_menu();
    } else {
        show_window_error("App tests failed.");
    }

    app_event_loop();

    return 0;
}
