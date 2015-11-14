#include <pebble.h>

#include "window_with_timer.h"
#include "../data/machine.h"

static TextLayer *editable_fields[F__COUNT];
static size_t current_field;

static Machine *current_machine;

static Workout *workout;

static Window *s_window;
static GFont s_res_bitham_30_black;
static GFont s_res_gothic_28_bold;
static GFont s_res_gothic_24;
static GFont s_res_gothic_24_bold;
static TextLayer *s_set_3;
static TextLayer *s_set_2;
static TextLayer *s_set_1;
static TextLayer *s_timer;
static TextLayer *s_rest_title;
static TextLayer *s_warmup_kg;
static TextLayer *s_labels_kg;
static TextLayer *s_machine;
static TextLayer *s_normal_kg;
static Layer *s_layer_1;

static void _initialise_ui(void) {
    s_window = window_create();

    s_res_bitham_30_black = fonts_get_system_font(FONT_KEY_BITHAM_30_BLACK);
    s_res_gothic_28_bold = fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD);
    s_res_gothic_24 = fonts_get_system_font(FONT_KEY_GOTHIC_24);
    s_res_gothic_24_bold = fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD);
    // s_set_3
    s_set_3 = text_layer_create(GRect(98, 88, 37, 35));
    text_layer_set_background_color(s_set_3, GColorClear);
    text_layer_set_text(s_set_3, "10");
    text_layer_set_text_alignment(s_set_3, GTextAlignmentRight);
    text_layer_set_font(s_set_3, s_res_bitham_30_black);
    layer_add_child(window_get_root_layer(s_window), (Layer *) s_set_3);

    // s_set_2
    s_set_2 = text_layer_create(GRect(54, 88, 36, 33));
    text_layer_set_background_color(s_set_2, GColorClear);
    text_layer_set_text(s_set_2, "14");
    text_layer_set_text_alignment(s_set_2, GTextAlignmentRight);
    text_layer_set_font(s_set_2, s_res_bitham_30_black);
    layer_add_child(window_get_root_layer(s_window), (Layer *) s_set_2);

    // s_set_1
    s_set_1 = text_layer_create(GRect(10, 88, 35, 33));
    text_layer_set_background_color(s_set_1, GColorClear);
    text_layer_set_text(s_set_1, "12");
    text_layer_set_text_alignment(s_set_1, GTextAlignmentRight);
    text_layer_set_font(s_set_1, s_res_bitham_30_black);
    layer_add_child(window_get_root_layer(s_window), (Layer *) s_set_1);

    // s_timer
    s_timer = text_layer_create(GRect(87, 122, 55, 31));
    text_layer_set_background_color(s_timer, GColorClear);
    text_layer_set_text(s_timer, "00:30");
    text_layer_set_font(s_timer, s_res_gothic_28_bold);
    layer_add_child(window_get_root_layer(s_window), (Layer *) s_timer);

    // s_rest_title
    s_rest_title = text_layer_create(GRect(2, 123, 75, 24));
    text_layer_set_background_color(s_rest_title, GColorClear);
    text_layer_set_text(s_rest_title, "set rest");
    text_layer_set_font(s_rest_title, s_res_gothic_24);
    layer_add_child(window_get_root_layer(s_window), (Layer *) s_rest_title);

    // s_warmup_kg
    s_warmup_kg = text_layer_create(GRect(0, 54, 54, 34));
    text_layer_set_background_color(s_warmup_kg, GColorClear);
    text_layer_set_text(s_warmup_kg, "125");
    text_layer_set_text_alignment(s_warmup_kg, GTextAlignmentRight);
    text_layer_set_font(s_warmup_kg, s_res_bitham_30_black);
    layer_add_child(window_get_root_layer(s_window), (Layer *) s_warmup_kg);

    // s_labels_kg
    s_labels_kg = text_layer_create(GRect(54, 63, 92, 31));
    text_layer_set_background_color(s_labels_kg, GColorClear);
    text_layer_set_text(s_labels_kg, "kg             kg");
    text_layer_set_font(s_labels_kg, s_res_gothic_24);
    layer_add_child(window_get_root_layer(s_window), (Layer *) s_labels_kg);

    // s_machine
    s_machine = text_layer_create(GRect(2, 18, 138, 42));
    text_layer_set_background_color(s_machine, GColorClear);
    text_layer_set_text(s_machine, "triceps is nice");
    text_layer_set_font(s_machine, s_res_gothic_24_bold);
    layer_add_child(window_get_root_layer(s_window), (Layer *) s_machine);

    // s_normal_kg
    s_normal_kg = text_layer_create(GRect(70, 54, 54, 31));
    text_layer_set_background_color(s_normal_kg, GColorClear);
    text_layer_set_text(s_normal_kg, "170");
    text_layer_set_text_alignment(s_normal_kg, GTextAlignmentRight);
    text_layer_set_font(s_normal_kg, s_res_bitham_30_black);
    layer_add_child(window_get_root_layer(s_window), (Layer *) s_normal_kg);

    // s_layer_1
    s_layer_1 = layer_create(GRect(7, 1, 134, 15));
    layer_add_child(window_get_root_layer(s_window), s_layer_1);
}

static void _destroy_ui(void) {
    window_destroy(s_window);
    text_layer_destroy(s_set_3);
    text_layer_destroy(s_set_2);
    text_layer_destroy(s_set_1);
    text_layer_destroy(s_timer);
    text_layer_destroy(s_rest_title);
    text_layer_destroy(s_warmup_kg);
    text_layer_destroy(s_labels_kg);
    text_layer_destroy(s_machine);
    text_layer_destroy(s_normal_kg);
    layer_destroy(s_layer_1);
}
// END AUTO-GENERATED UI CODE

static void _handle_window_unload(Window *window) {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "unload called, ui was destroyed");
    _destroy_ui();
    workout_destroy(workout);
}

static void _update_machine_layers() {
    text_layer_set_text(s_machine, current_machine->title);

    snprintf(current_machine->warmup_kg_str, 4, "%d", current_machine->warmup_kg);
    snprintf(current_machine->normal_kg_str, 4, "%d", current_machine->normal_kg);
    snprintf(current_machine->set_1_str, 4, "%d", current_machine->set_1);
    snprintf(current_machine->set_2_str, 4, "%d", current_machine->set_2);
    snprintf(current_machine->set_3_str, 4, "%d", current_machine->set_3);

    text_layer_set_text(s_warmup_kg, current_machine->warmup_kg_str);
    text_layer_set_text(s_normal_kg, current_machine->normal_kg_str);
    text_layer_set_text(s_set_1, current_machine->set_1_str);
    text_layer_set_text(s_set_2, current_machine->set_2_str);
    text_layer_set_text(s_set_3, current_machine->set_3_str);

    layer_mark_dirty(s_layer_1);
}

static void _clear_inv_layer() {
    text_layer_set_background_color(editable_fields[current_field], GColorClear);
    text_layer_set_text_color(editable_fields[current_field], GColorBlack);
}

static void _update_inv_layer() {
    text_layer_set_background_color(editable_fields[current_field], GColorDarkGreen);
    text_layer_set_text_color(editable_fields[current_field], GColorWhite);
}

static void _decrease_click_handler(ClickRecognizerRef recognizer, void *context) {
    switch (current_field) {
        case F_TITLE:
            if (current_machine->next != NULL) {
                current_machine = current_machine->next;
                _update_machine_layers();
            }
            break;
        case F_WARMUP_KG:
            current_machine->warmup_kg--;
            break;
        case F_NORMAL_KG:
            current_machine->normal_kg--;
            break;
        case F_SET_1:
            current_machine->set_1--;
            break;
        case F_SET_2:
            current_machine->set_2--;
            break;
        case F_SET_3:
            current_machine->set_3--;
            break;
        default:
            return;
    }
    _update_machine_layers();
}

static void _increase_click_handler(ClickRecognizerRef recognizer, void *context) {
    switch (current_field) {
        case F_TITLE:
            if (current_machine->prev != NULL) {
                current_machine = current_machine->prev;
                _update_machine_layers();
            }
            break;
        case F_WARMUP_KG:
            current_machine->warmup_kg++;
            break;
        case F_NORMAL_KG:
            current_machine->normal_kg++;
            break;
        case F_SET_1:
            current_machine->set_1++;
            break;
        case F_SET_2:
            current_machine->set_2++;
            break;
        case F_SET_3:
            current_machine->set_3++;
            break;
        default:
            return;
    }
    _update_machine_layers();
}

static void _hide_window_with_timer(void) {
    window_stack_remove(s_window, true);
}

static void _prev_click_handler(ClickRecognizerRef recognizer, void *context) {
    if (current_field == 0) {
        _hide_window_with_timer();
        return;
    }
    if (current_field > 0) {
        _clear_inv_layer();
        current_field--;
        _update_inv_layer();
    }
}

static void _next_click_handler(ClickRecognizerRef recognizer, void *context) {
    _clear_inv_layer();
    current_field++;
    if (current_field == F__COUNT) {
        current_field = 0;

        current_machine->is_done = true;
        if (current_machine->time_done == 0) {
            current_machine->time_done = time(NULL);
        }
        machine_save_current(current_machine);

        if (current_machine->next != NULL) {
            current_machine = current_machine->next;
        } else {
            if (workout->time_end == 0) {
                workout->time_end = (long) time(NULL);
                workout_save_current(workout, /*deep*/false);
            }
        }
        _update_machine_layers();
    }

    _update_inv_layer();
}

static void _click_config_provider(void *context) {
    window_single_click_subscribe(BUTTON_ID_SELECT, _next_click_handler);
    window_single_click_subscribe(BUTTON_ID_BACK, _prev_click_handler);

    window_single_repeating_click_subscribe(BUTTON_ID_UP, 100, _increase_click_handler);
    window_single_repeating_click_subscribe(BUTTON_ID_DOWN, 100, _decrease_click_handler);
}

static void _draw_rectangles(struct Layer *layer, GContext *ctx) {
    int16_t left_pos = 4;
    int16_t top_pos = 3;
    Machine *m = workout->first_machine;
    while (m != NULL) {
        GRect rect;
        bool double_border = false;
        if (current_machine == m) {
            rect = (GRect) {.origin = {(int16_t) (left_pos - 4), (int16_t) (top_pos - 3)}, .size = {9 + 8, 9 + 6}};
            double_border = true;
        } else {
            rect = (GRect) {.origin = {left_pos, top_pos}, .size = {9, 9}};
        }
        if (m->is_done) {
            graphics_fill_rect(ctx, rect, 0, GCornerNone);
        } else {
            graphics_draw_rect(ctx, rect);
            if (double_border) {
                GRect rect_double = (GRect) {.origin = {(int16_t) (rect.origin.x + 1), (int16_t) (rect.origin.y + 1)}, .size = {(int16_t) (rect.size.w - 2), (int16_t) (rect.size.h - 2)}};
                graphics_draw_rect(ctx, rect_double);
            }
        }

        left_pos += 11;
        m = m->next;
    }
}

void show_window_with_timer(bool new_workout, char location) {
    _initialise_ui();
    window_set_window_handlers(s_window, (WindowHandlers) {
            .unload = _handle_window_unload,
    });

    window_set_click_config_provider(s_window, _click_config_provider);
    layer_set_update_proc(s_layer_1, _draw_rectangles);

    editable_fields[F_TITLE] = s_machine;
    editable_fields[F_WARMUP_KG] = s_warmup_kg;
    editable_fields[F_NORMAL_KG] = s_normal_kg;
    editable_fields[F_SET_1] = s_set_1;
    editable_fields[F_SET_2] = s_set_2;
    editable_fields[F_SET_3] = s_set_3;

    current_field = F_TITLE;

    workout = workout_create();
    workout_load_current(workout);

    if (new_workout) {
        workout->location = location;
        workout->time_start = time(NULL);

        workout_save_current(workout, /*deep*/false);
    }

    APP_LOG(APP_LOG_LEVEL_DEBUG, "Workout time: %ld - %ld, location: %c", workout->time_start, workout->time_end, workout->location);

    current_machine = workout->first_machine;

    _update_machine_layers();
    _update_inv_layer();

    window_stack_push(s_window, true);
}
