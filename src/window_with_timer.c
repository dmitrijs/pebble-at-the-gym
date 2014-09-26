#include "window_with_timer.h"
#include "enums.h"
#include "window_edit_number.h"
#include <pebble.h>

enum MACHINE_TYPES {
    M_WARMUP = 0x10,
    M_SHOULDERS,
    M_CHEST,
    M_TRICEPS,
    M_BICEPS,
    M_UPPER_BACK,
    M_LOWER_BACK,
    M_LEGS_ULTIMATE,
    M_LEGS_UP,
    M_LEGS_DOWN,
    M_ABS,
    M_ABS_SIDE,
    M_COOLDOWN
};
enum FIELD_TYPE {
    F_TITLE = 0, /* should be 0 */
    F_WARMUP_KG,
    F_NORMAL_KG,
    F_SET_1,
    F_SET_2,
    F_SET_3,
    F__COUNT
};

Layer* editable_fields[F__COUNT];
size_t current_field;

typedef struct Machine Machine;
struct Machine {
    int mkey;
    char *title;
    int warmup_kg;
    int normal_kg;
    int set_1;
    int set_2;
    int set_3;

    char* warmup_kg_str;
    char* normal_kg_str;
    char* set_1_str;
    char* set_2_str;
    char* set_3_str;

    Machine *next;
};

Machine* current_machine;

InverterLayer* s_invert_all;

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
    APP_LOG(APP_LOG_LEVEL_DEBUG, "unload called, ui was destroyed");
    destroy_ui();
}

static void update_machine() {
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
}

static void edit_number_callback(int key, int number) {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "callback: key=%d, number=%d", key, number);

    switch (current_field) {
        case F_WARMUP_KG:
            current_machine->warmup_kg = number;
            break;
        case F_NORMAL_KG:
            current_machine->normal_kg = number;
            break;
        case F_SET_1:
            current_machine->set_1 = number;
            break;
        case F_SET_2:
            current_machine->set_2 = number;
            break;
        case F_SET_3:
            current_machine->set_3 = number;
            break;
        default:
            return;
    }

    update_machine();
}

static void update_inv_layer() {
    GRect rect = layer_get_frame(editable_fields[current_field]);
    layer_set_frame((Layer *) s_inv_selector, rect);
}

static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
    int value_to_edit;
    switch (current_field) {
        case F_WARMUP_KG:
            value_to_edit = current_machine->warmup_kg;
            break;
        case F_NORMAL_KG:
            value_to_edit = current_machine->normal_kg;
            break;
        case F_SET_1:
            value_to_edit = current_machine->set_1;
            break;
        case F_SET_2:
            value_to_edit = current_machine->set_2;
            break;
        case F_SET_3:
            value_to_edit = current_machine->set_3;
            break;
        default:
            return;
    }

    show_window_edit_number(0, value_to_edit, edit_number_callback);
}

static void up_click_handler(ClickRecognizerRef recognizer, void *context) {
    if (current_field == 0) {
        current_field = F__COUNT - 1;
    } else {
        current_field--;
    }
    update_inv_layer();
}

static void down_click_handler(ClickRecognizerRef recognizer, void *context) {
    if (current_field == F__COUNT - 1) {
        current_field = 0;
    } else {
        current_field++;
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

    editable_fields[F_TITLE] = (Layer *) s_machine;
    editable_fields[F_WARMUP_KG] = (Layer *) s_warmup_kg;
    editable_fields[F_NORMAL_KG] = (Layer *) s_normal_kg;
    editable_fields[F_SET_1] = (Layer *) s_set_1;
    editable_fields[F_SET_2] = (Layer *) s_set_2;
    editable_fields[F_SET_3] = (Layer *) s_set_3;

    current_field = F_SET_1;

    current_machine = malloc(sizeof(Machine));
    current_machine->mkey = M_TRICEPS;
    current_machine->next = NULL;

    current_machine->title = "Triceps is no joke; mister";
    current_machine->warmup_kg = 105;
    current_machine->normal_kg = 158;
    current_machine->set_1 = 11;
    current_machine->set_2 = 12;
    current_machine->set_3 = 13;

    APP_LOG(APP_LOG_LEVEL_DEBUG, "warmpup init = %d", (*current_machine).warmup_kg);

    current_machine->warmup_kg_str = malloc(4);
    current_machine->normal_kg_str = malloc(4);
    current_machine->set_1_str = malloc(4);
    current_machine->set_2_str = malloc(4);
    current_machine->set_3_str = malloc(4);

    update_machine();

    update_inv_layer();

    window_stack_push(s_window, true);

//    s_invert_all = inverter_layer_create(layer_get_bounds(window_get_root_layer(s_window)));
//    layer_add_child(window_get_root_layer(s_window), (Layer *) s_invert_all);
}

void hide_window_with_timer(void) {
    window_stack_remove(s_window, true);
}
