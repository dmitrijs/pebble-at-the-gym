#include "window_prepare.h"
#include <pebble.h>

// BEGIN AUTO-GENERATED UI CODE; DO NOT MODIFY
static Window *s_window;
static GBitmap *s_res_prepare_bg;
static BitmapLayer *s_prepare;

static void initialise_ui(void) {
    s_window = window_create();
    window_set_fullscreen(s_window, false);

    s_res_prepare_bg = gbitmap_create_with_resource(RESOURCE_ID_PREPARE_BG);
    // s_prepare
    s_prepare = bitmap_layer_create(GRect(0, 0, 144, 152));
    bitmap_layer_set_bitmap(s_prepare, s_res_prepare_bg);
    bitmap_layer_set_background_color(s_prepare, GColorWhite);
    layer_add_child(window_get_root_layer(s_window), (Layer *) s_prepare);
}

static void destroy_ui(void) {
    window_destroy(s_window);
    bitmap_layer_destroy(s_prepare);
    gbitmap_destroy(s_res_prepare_bg);
}
// END AUTO-GENERATED UI CODE

static void handle_window_unload(Window *window) {
    destroy_ui();
}

void show_window_prepare(void) {
    initialise_ui();
    window_set_window_handlers(s_window, (WindowHandlers) {
            .unload = handle_window_unload,
    });
    window_stack_push(s_window, true);
}

void hide_window_prepare(void) {
    window_stack_remove(s_window, true);
}
