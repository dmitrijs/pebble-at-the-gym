#include "window_delete.h"
#include "../data/machine.h"

static Window *s_window;
static GFont s_res_gothic_18_bold;
static TextLayer *s_textlayer_1;
static TextLayer *s_textlayer_2;
static TextLayer *s_textlayer_3;
static TextLayer *s_textlayer_4;

static void initialise_ui(void) {
    s_window = window_create();

    s_res_gothic_18_bold = fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD);
    // s_textlayer_1
    s_textlayer_1 = text_layer_create(GRect(2, 1, 141, 40));
    text_layer_set_text(s_textlayer_1, "Press 'select' to delete workouts.");
    text_layer_set_text_alignment(s_textlayer_1, GTextAlignmentCenter);
    text_layer_set_font(s_textlayer_1, s_res_gothic_18_bold);
    layer_add_child(window_get_root_layer(s_window), (Layer *) s_textlayer_1);

    // s_textlayer_2
    s_textlayer_2 = text_layer_create(GRect(0, 43, 144, 20));
    text_layer_set_text(s_textlayer_2, "Slot #1");
    text_layer_set_text_alignment(s_textlayer_2, GTextAlignmentCenter);
    layer_add_child(window_get_root_layer(s_window), (Layer *) s_textlayer_2);

    // s_textlayer_3
    s_textlayer_3 = text_layer_create(GRect(0, 77, 144, 20));
    text_layer_set_text(s_textlayer_3, "Slot #2");
    text_layer_set_text_alignment(s_textlayer_3, GTextAlignmentCenter);
    layer_add_child(window_get_root_layer(s_window), (Layer *) s_textlayer_3);

    // s_textlayer_4
    s_textlayer_4 = text_layer_create(GRect(0, 111, 144, 20));
    text_layer_set_text(s_textlayer_4, "Slot #3");
    text_layer_set_text_alignment(s_textlayer_4, GTextAlignmentCenter);
    layer_add_child(window_get_root_layer(s_window), (Layer *) s_textlayer_4);
}

static void destroy_ui(void) {
    text_layer_destroy(s_textlayer_1);
    text_layer_destroy(s_textlayer_2);
    text_layer_destroy(s_textlayer_3);
    text_layer_destroy(s_textlayer_4);
}

static void handle_window_unload(Window *window) {
    destroy_ui();
}

static bool delete_in_progress[3];

void do_delete() {
    SaveState state = slots_load_state();
    if (state.save1_in_use) {
        workout_delete_by_slot(1);
    }
    if (state.save2_in_use) {
        workout_delete_by_slot(2);
    }
    if (state.save3_in_use) {
        workout_delete_by_slot(3);
    }

    for (int i = 0; i < 3; ++i) {
        delete_in_progress[i] = false;
    }
}

void initialize_delete_info_texts() {
    SaveState state = slots_load_state();
    if (!state.save1_in_use) {
        layer_remove_from_parent(text_layer_get_layer(s_textlayer_2));
    }
    if (!state.save2_in_use) {
        layer_remove_from_parent(text_layer_get_layer(s_textlayer_3));
    }
    if (!state.save3_in_use) {
        layer_remove_from_parent(text_layer_get_layer(s_textlayer_4));
    }
}

static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
    text_layer_set_text(s_textlayer_1, "Deleting workouts,\nplease wait...");

    do_delete();
}

static void prev_click_handler(ClickRecognizerRef recognizer, void *context) {
    hide_window_delete();
}

static void click_config_provider(void *context) {
    window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
    window_single_click_subscribe(BUTTON_ID_BACK, prev_click_handler);
}

void show_window_delete(void) {
    initialise_ui();
    initialize_delete_info_texts();

    window_set_click_config_provider(s_window, click_config_provider);

    window_set_window_handlers(s_window, (WindowHandlers) {
            .unload = handle_window_unload,
    });
    window_stack_push(s_window, true);
}

void hide_window_delete(void) {
    window_stack_remove(s_window, true);
}
