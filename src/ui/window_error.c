#include "window_error.h"
#include <pebble.h>

static Window *s_window;
static GFont s_res_gothic_24_bold;
static TextLayer *s_textlayer_1;

static void _initialise_ui(const char *msg) {
    s_window = window_create();

    s_res_gothic_24_bold = fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD);
    // s_textlayer_1
    s_textlayer_1 = text_layer_create(GRect(5, 17, 134, 122));
    text_layer_set_text(s_textlayer_1, msg);
    text_layer_set_text_alignment(s_textlayer_1, GTextAlignmentCenter);
    text_layer_set_font(s_textlayer_1, s_res_gothic_24_bold);
    layer_add_child(window_get_root_layer(s_window), (Layer *) s_textlayer_1);
}

static void _destroy_ui(void) {
    window_destroy(s_window);
    text_layer_destroy(s_textlayer_1);
}

static void _handle_window_unload(Window *window) {
    _destroy_ui();
}

void show_window_error(const char *msg) {
    _initialise_ui(msg);
    window_set_window_handlers(s_window, (WindowHandlers) {
            .unload = _handle_window_unload,
    });
    window_stack_push(s_window, true);
}
