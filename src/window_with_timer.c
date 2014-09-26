#include "window_with_timer.h"
#include <pebble.h>

// BEGIN AUTO-GENERATED UI CODE; DO NOT MODIFY
static Window *s_window;
static GFont s_res_bitham_30_black;
static GFont s_res_gothic_28_bold;
static GFont s_res_gothic_24;
static GBitmap *s_res_sq_sel;
static TextLayer *s_textlayer_7;
static TextLayer *s_textlayer_1;
static TextLayer *s_textlayer_2;
static TextLayer *s_textlayer_5;
static TextLayer *s_textlayer_6;
static InverterLayer *s_inverterlayer_3;
static TextLayer *s_textlayer_3;
static BitmapLayer *s_bitmaplayer_1;
static InverterLayer *s_inverterlayer_1;

static void initialise_ui(void) {
    s_window = window_create();
    window_set_fullscreen(s_window, false);

    s_res_bitham_30_black = fonts_get_system_font(FONT_KEY_BITHAM_30_BLACK);
    s_res_gothic_28_bold = fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD);
    s_res_gothic_24 = fonts_get_system_font(FONT_KEY_GOTHIC_24);
    s_res_sq_sel = gbitmap_create_with_resource(RESOURCE_ID_SQUARES_SELECTED);
    // s_textlayer_7
    s_textlayer_7 = text_layer_create(GRect(13, 87, 129, 33));
    text_layer_set_background_color(s_textlayer_7, GColorClear);
    text_layer_set_text(s_textlayer_7, "12  12  8");
    text_layer_set_font(s_textlayer_7, s_res_bitham_30_black);
    layer_add_child(window_get_root_layer(s_window), (Layer *)s_textlayer_7);

    // s_textlayer_1
    s_textlayer_1 = text_layer_create(GRect(87, 122, 55, 31));
    text_layer_set_background_color(s_textlayer_1, GColorClear);
    text_layer_set_text(s_textlayer_1, "00:30");
    text_layer_set_font(s_textlayer_1, s_res_gothic_28_bold);
    layer_add_child(window_get_root_layer(s_window), (Layer *)s_textlayer_1);

    // s_textlayer_2
    s_textlayer_2 = text_layer_create(GRect(2, 123, 75, 24));
    text_layer_set_background_color(s_textlayer_2, GColorClear);
    text_layer_set_text(s_textlayer_2, "set rest");
    text_layer_set_font(s_textlayer_2, s_res_gothic_24);
    layer_add_child(window_get_root_layer(s_window), (Layer *)s_textlayer_2);

    // s_textlayer_5
    s_textlayer_5 = text_layer_create(GRect(12, 54, 114, 34));
    text_layer_set_background_color(s_textlayer_5, GColorClear);
    text_layer_set_text(s_textlayer_5, "25   70");
    text_layer_set_font(s_textlayer_5, s_res_bitham_30_black);
    layer_add_child(window_get_root_layer(s_window), (Layer *)s_textlayer_5);

    // s_textlayer_6
    s_textlayer_6 = text_layer_create(GRect(50, 62, 94, 31));
    text_layer_set_background_color(s_textlayer_6, GColorClear);
    text_layer_set_text(s_textlayer_6, "kg            kg");
    text_layer_set_font(s_textlayer_6, s_res_gothic_24);
    layer_add_child(window_get_root_layer(s_window), (Layer *)s_textlayer_6);

    // s_inverterlayer_3
    s_inverterlayer_3 = inverter_layer_create(GRect(9, 88, 43, 35));
    layer_add_child(window_get_root_layer(s_window), (Layer *)s_inverterlayer_3);

    // s_textlayer_3
    s_textlayer_3 = text_layer_create(GRect(4, 9, 139, 56));
    text_layer_set_background_color(s_textlayer_3, GColorClear);
    text_layer_set_text(s_textlayer_3, "triceps where you straighten hands down");
    text_layer_set_font(s_textlayer_3, s_res_gothic_24);
    layer_add_child(window_get_root_layer(s_window), (Layer *)s_textlayer_3);

    // s_bitmaplayer_1
    s_bitmaplayer_1 = bitmap_layer_create(GRect(1, 1, 141, 13));
    bitmap_layer_set_bitmap(s_bitmaplayer_1, s_res_sq_sel);
    layer_add_child(window_get_root_layer(s_window), (Layer *)s_bitmaplayer_1);

    // s_inverterlayer_1
    s_inverterlayer_1 = inverter_layer_create(GRect(0, 124, 144, 28));
    layer_add_child(window_get_root_layer(s_window), (Layer *)s_inverterlayer_1);
}

static void destroy_ui(void) {
    window_destroy(s_window);
    text_layer_destroy(s_textlayer_7);
    text_layer_destroy(s_textlayer_1);
    text_layer_destroy(s_textlayer_2);
    text_layer_destroy(s_textlayer_5);
    text_layer_destroy(s_textlayer_6);
    inverter_layer_destroy(s_inverterlayer_3);
    text_layer_destroy(s_textlayer_3);
    bitmap_layer_destroy(s_bitmaplayer_1);
    inverter_layer_destroy(s_inverterlayer_1);
    gbitmap_destroy(s_res_sq_sel);
}
// END AUTO-GENERATED UI CODE

static void handle_window_unload(Window* window) {
    destroy_ui();
}

void show_window_with_timer(void) {
    initialise_ui();
    window_set_window_handlers(s_window, (WindowHandlers) {
            .unload = handle_window_unload,
    });
    window_stack_push(s_window, true);
}

void hide_window_with_timer(void) {
    window_stack_remove(s_window, true);
}
