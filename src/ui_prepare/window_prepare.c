#include "window_prepare.h"
#include <pebble.h>

// BEGIN AUTO-GENERATED UI CODE; DO NOT MODIFY
static Window *s_window;
static GBitmap *s_res_prepare_bg;
static BitmapLayer *s_prepare;

static void _initialise_ui(void) {
    s_window = window_create();

    s_res_prepare_bg = gbitmap_create_with_resource(RESOURCE_ID_PREPARE_BG);
    // s_prepare
    s_prepare = bitmap_layer_create(GRect(0, 0, 144, 152));
    bitmap_layer_set_bitmap(s_prepare, s_res_prepare_bg);
    bitmap_layer_set_background_color(s_prepare, GColorWhite);
    layer_add_child(window_get_root_layer(s_window), (Layer *) s_prepare);
}

static void _destroy_ui(void) {
    window_destroy(s_window);
    bitmap_layer_destroy(s_prepare);
    gbitmap_destroy(s_res_prepare_bg);
}
// END AUTO-GENERATED UI CODE

static void _handle_window_unload(Window *window) {
    _destroy_ui();
}

void show_window_prepare(void) {
    _initialise_ui();
    window_set_window_handlers(s_window, (WindowHandlers) {
            .unload = _handle_window_unload,
    });
    window_stack_push(s_window, true);
}
