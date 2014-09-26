#include "window_edit_number.h"
#include <pebble.h>

static int the_number;
static int the_key;
static EditNumberCallback the_callback;

static char the_number_text[10];

// BEGIN AUTO-GENERATED UI CODE; DO NOT MODIFY
static Window *s_window;
static GBitmap *s_res_up;
static GBitmap *s_res_ok;
static GBitmap *s_res_down;
static GFont s_res_roboto_bold_subset_49;
static TextLayer *s_textlayer_1;
static ActionBarLayer *s_actionbarlayer_1;

static void initialise_ui(void) {
    s_window = window_create();
    window_set_background_color(s_window, GColorBlack);
    window_set_fullscreen(s_window, false);

    s_res_up = gbitmap_create_with_resource(RESOURCE_ID_UP);
    s_res_ok = gbitmap_create_with_resource(RESOURCE_ID_OK);
    s_res_down = gbitmap_create_with_resource(RESOURCE_ID_DOWN);
    s_res_roboto_bold_subset_49 = fonts_get_system_font(FONT_KEY_ROBOTO_BOLD_SUBSET_49);
    // s_textlayer_1
    s_textlayer_1 = text_layer_create(GRect(25, 39, 92, 70));
    text_layer_set_background_color(s_textlayer_1, GColorBlack);
    text_layer_set_text_color(s_textlayer_1, GColorWhite);
    text_layer_set_text(s_textlayer_1, "12");
    text_layer_set_text_alignment(s_textlayer_1, GTextAlignmentCenter);
    text_layer_set_font(s_textlayer_1, s_res_roboto_bold_subset_49);
    layer_add_child(window_get_root_layer(s_window), (Layer *)s_textlayer_1);

    // s_actionbarlayer_1
    s_actionbarlayer_1 = action_bar_layer_create();
    action_bar_layer_add_to_window(s_actionbarlayer_1, s_window);
    action_bar_layer_set_background_color(s_actionbarlayer_1, GColorBlack);
    action_bar_layer_set_icon(s_actionbarlayer_1, BUTTON_ID_UP, s_res_up);
    action_bar_layer_set_icon(s_actionbarlayer_1, BUTTON_ID_SELECT, s_res_ok);
    action_bar_layer_set_icon(s_actionbarlayer_1, BUTTON_ID_DOWN, s_res_down);
    layer_add_child(window_get_root_layer(s_window), (Layer *)s_actionbarlayer_1);
}

static void destroy_ui(void) {
    window_destroy(s_window);
    text_layer_destroy(s_textlayer_1);
    action_bar_layer_destroy(s_actionbarlayer_1);
    gbitmap_destroy(s_res_up);
    gbitmap_destroy(s_res_ok);
    gbitmap_destroy(s_res_down);
}
// END AUTO-GENERATED UI CODE

static void handle_window_unload(Window* window) {
    destroy_ui();
}

static void update_ui(void) {
    snprintf(the_number_text, sizeof(the_number_text), "%d", the_number);
    text_layer_set_text(s_textlayer_1, the_number_text);
}

static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
    the_callback(the_key, the_number);
    hide_window_edit_number();
}

static void up_click_handler(ClickRecognizerRef recognizer, void *context) {
    the_number++;
    update_ui();
}

static void down_click_handler(ClickRecognizerRef recognizer, void *context) {
    the_number--;
    update_ui();
}

static void click_config_provider(void* context) {
    window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
    window_single_click_subscribe(BUTTON_ID_UP, up_click_handler);
    window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler);
}

void show_window_edit_number(int key, int number, EditNumberCallback callback) {
    the_key = key;
    the_number = number;
    the_callback = callback;

    initialise_ui();
    window_set_window_handlers(s_window, (WindowHandlers) {
            .unload = handle_window_unload,
    });

    action_bar_layer_set_click_config_provider(s_actionbarlayer_1, click_config_provider);

    window_stack_push(s_window, true);
}

void hide_window_edit_number(void) {
    window_stack_remove(s_window, true);
}
