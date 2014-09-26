#include "window_with_timer.h"
#include "enums.h"
#include "window_edit_number.h"
#include <pebble.h>

#define EDITABLE_FIELDS_SIZE 6

Layer* editable_fields[EDITABLE_FIELDS_SIZE];
size_t selected_field_index;

// BEGIN AUTO-GENERATED UI CODE; DO NOT MODIFY
static Window *s_window;
static GFont s_res_bitham_30_black;
static GFont s_res_gothic_28_bold;
static GFont s_res_gothic_24;
static GBitmap *s_res_squares_selected;
static TextLayer *s_set_3;
static TextLayer *s_set_2;
static TextLayer *s_set_1;
static TextLayer *s_timer;
static TextLayer *s_rest_title;
static TextLayer *s_warmup_kg;
static TextLayer *s_labels_kg;
static TextLayer *s_machine;
static BitmapLayer *s_bitmap_progress;
static InverterLayer *s_inv_timer;
static TextLayer *s_normal_kg;
static InverterLayer *s_inv_selector;

static void initialise_ui(void) {
    s_window = window_create();
    window_set_fullscreen(s_window, false);

    s_res_bitham_30_black = fonts_get_system_font(FONT_KEY_BITHAM_30_BLACK);
    s_res_gothic_28_bold = fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD);
    s_res_gothic_24 = fonts_get_system_font(FONT_KEY_GOTHIC_24);
    s_res_squares_selected = gbitmap_create_with_resource(RESOURCE_ID_SQUARES_SELECTED);
    // s_set_3
    s_set_3 = text_layer_create(GRect(98, 88, 37, 35));
    text_layer_set_background_color(s_set_3, GColorClear);
    text_layer_set_text(s_set_3, "10");
    text_layer_set_text_alignment(s_set_3, GTextAlignmentRight);
    text_layer_set_font(s_set_3, s_res_bitham_30_black);
    layer_add_child(window_get_root_layer(s_window), (Layer *)s_set_3);

    // s_set_2
    s_set_2 = text_layer_create(GRect(54, 88, 36, 33));
    text_layer_set_background_color(s_set_2, GColorClear);
    text_layer_set_text(s_set_2, "14");
    text_layer_set_text_alignment(s_set_2, GTextAlignmentRight);
    text_layer_set_font(s_set_2, s_res_bitham_30_black);
    layer_add_child(window_get_root_layer(s_window), (Layer *)s_set_2);

    // s_set_1
    s_set_1 = text_layer_create(GRect(10, 88, 35, 33));
    text_layer_set_background_color(s_set_1, GColorClear);
    text_layer_set_text(s_set_1, "12");
    text_layer_set_text_alignment(s_set_1, GTextAlignmentRight);
    text_layer_set_font(s_set_1, s_res_bitham_30_black);
    layer_add_child(window_get_root_layer(s_window), (Layer *)s_set_1);

    // s_timer
    s_timer = text_layer_create(GRect(87, 122, 55, 31));
    text_layer_set_background_color(s_timer, GColorClear);
    text_layer_set_text(s_timer, "00:30");
    text_layer_set_font(s_timer, s_res_gothic_28_bold);
    layer_add_child(window_get_root_layer(s_window), (Layer *)s_timer);

    // s_rest_title
    s_rest_title = text_layer_create(GRect(2, 123, 75, 24));
    text_layer_set_background_color(s_rest_title, GColorClear);
    text_layer_set_text(s_rest_title, "set rest");
    text_layer_set_font(s_rest_title, s_res_gothic_24);
    layer_add_child(window_get_root_layer(s_window), (Layer *)s_rest_title);

    // s_warmup_kg
    s_warmup_kg = text_layer_create(GRect(0, 54, 54, 34));
    text_layer_set_background_color(s_warmup_kg, GColorClear);
    text_layer_set_text(s_warmup_kg, "125");
    text_layer_set_text_alignment(s_warmup_kg, GTextAlignmentRight);
    text_layer_set_font(s_warmup_kg, s_res_bitham_30_black);
    layer_add_child(window_get_root_layer(s_window), (Layer *)s_warmup_kg);

    // s_labels_kg
    s_labels_kg = text_layer_create(GRect(54, 63, 92, 31));
    text_layer_set_background_color(s_labels_kg, GColorClear);
    text_layer_set_text(s_labels_kg, "kg             kg");
    text_layer_set_font(s_labels_kg, s_res_gothic_24);
    layer_add_child(window_get_root_layer(s_window), (Layer *)s_labels_kg);

    // s_machine
    s_machine = text_layer_create(GRect(4, 9, 139, 56));
    text_layer_set_background_color(s_machine, GColorClear);
    text_layer_set_text(s_machine, "triceps where you straighten hands");
    text_layer_set_font(s_machine, s_res_gothic_24);
    layer_add_child(window_get_root_layer(s_window), (Layer *)s_machine);

    // s_bitmap_progress
    s_bitmap_progress = bitmap_layer_create(GRect(2, 1, 141, 13));
    bitmap_layer_set_bitmap(s_bitmap_progress, s_res_squares_selected);
    layer_add_child(window_get_root_layer(s_window), (Layer *)s_bitmap_progress);

    // s_inv_timer
    s_inv_timer = inverter_layer_create(GRect(0, 124, 144, 28));
    layer_add_child(window_get_root_layer(s_window), (Layer *)s_inv_timer);

    // s_normal_kg
    s_normal_kg = text_layer_create(GRect(70, 54, 54, 31));
    text_layer_set_background_color(s_normal_kg, GColorClear);
    text_layer_set_text(s_normal_kg, "170");
    text_layer_set_text_alignment(s_normal_kg, GTextAlignmentRight);
    text_layer_set_font(s_normal_kg, s_res_bitham_30_black);
    layer_add_child(window_get_root_layer(s_window), (Layer *)s_normal_kg);

    // s_inv_selector
    s_inv_selector = inverter_layer_create(GRect(8, 88, 43, 35));
    layer_add_child(window_get_root_layer(s_window), (Layer *)s_inv_selector);
}

static void destroy_ui(void) {
    window_destroy(s_window);
    text_layer_destroy(s_set_3);
    text_layer_destroy(s_set_2);
    text_layer_destroy(s_set_1);
    text_layer_destroy(s_timer);
    text_layer_destroy(s_rest_title);
    text_layer_destroy(s_warmup_kg);
    text_layer_destroy(s_labels_kg);
    text_layer_destroy(s_machine);
    bitmap_layer_destroy(s_bitmap_progress);
    inverter_layer_destroy(s_inv_timer);
    text_layer_destroy(s_normal_kg);
    inverter_layer_destroy(s_inv_selector);
    gbitmap_destroy(s_res_squares_selected);
}
// END AUTO-GENERATED UI CODE

static void handle_window_unload(Window* window) {
    destroy_ui();
}

static void edit_number_callback(int key, int number) {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "callback: key=%d, number=%d", key, number);
}

static void update_inv_layer() {
    GRect rect = layer_get_frame(editable_fields[selected_field_index]);
    layer_set_frame((Layer *) s_inv_selector, rect);
}

static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
    show_window_edit_number(0, 0, NULL);
}

static void up_click_handler(ClickRecognizerRef recognizer, void *context) {
    if (selected_field_index == 0) {
        selected_field_index = EDITABLE_FIELDS_SIZE - 1;
    } else {
        selected_field_index--;
    }
    update_inv_layer();
}

static void down_click_handler(ClickRecognizerRef recognizer, void *context) {
    if (selected_field_index == EDITABLE_FIELDS_SIZE - 1) {
        selected_field_index = 0;
    } else {
        selected_field_index++;
    }
    update_inv_layer();
}

static void click_config_provider(void *context) {
    window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
    window_single_click_subscribe(BUTTON_ID_UP, up_click_handler);
    window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler);
}

void show_window_with_timer(void) {
    initialise_ui();
    window_set_window_handlers(s_window, (WindowHandlers) {
            .unload = handle_window_unload,
    });

    window_set_click_config_provider(s_window, click_config_provider);

    editable_fields[0] = (Layer *) s_machine;
    editable_fields[1] = (Layer *) s_warmup_kg;
    editable_fields[2] = (Layer *) s_normal_kg;
    editable_fields[3] = (Layer *) s_set_1;
    editable_fields[4] = (Layer *) s_set_2;
    editable_fields[5] = (Layer *) s_set_3;

    selected_field_index = 0;

    update_inv_layer();

    window_stack_push(s_window, true);
}

void hide_window_with_timer(void) {
    window_stack_remove(s_window, true);
}
