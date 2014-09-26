#include "window_with_timer.h"
#include "enums.h"
#include "lib/parsed.h"
#include <pebble.h>

enum MACHINE_TYPES {
    M_WARMUP,
    M_SHOULDERS,
    M_CHEST,
    M_TRICEPS,
    M_BICEPS,
    M_UPPER_BACK,
    M_LOWER_BACK,
    M_LEGS_ULTIMATE,
//    M_LEGS_UP, /* excluded for now */
//    M_LEGS_DOWN,
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
enum {
    DATA_MACHINES = 0
};
char data_buffer[300];

Layer *editable_fields[F__COUNT];
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
    bool is_done;

    char *warmup_kg_str;
    char *normal_kg_str;
    char *set_1_str;
    char *set_2_str;
    char *set_3_str;

    Machine *next;
    Machine *prev;
};

Machine *first_machine = NULL;
Machine *current_machine;

InverterLayer *s_invert_all;

// BEGIN AUTO-GENERATED UI CODE; DO NOT MODIFY
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
static InverterLayer *s_inv_timer;
static TextLayer *s_normal_kg;
static InverterLayer *s_inv_selector;
static Layer *s_layer_1;

static void initialise_ui(void) {
    s_window = window_create();
    window_set_fullscreen(s_window, false);

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

    // s_inv_timer
    s_inv_timer = inverter_layer_create(GRect(0, 124, 144, 28));
    layer_add_child(window_get_root_layer(s_window), (Layer *) s_inv_timer);

    // s_normal_kg
    s_normal_kg = text_layer_create(GRect(70, 54, 54, 31));
    text_layer_set_background_color(s_normal_kg, GColorClear);
    text_layer_set_text(s_normal_kg, "170");
    text_layer_set_text_alignment(s_normal_kg, GTextAlignmentRight);
    text_layer_set_font(s_normal_kg, s_res_bitham_30_black);
    layer_add_child(window_get_root_layer(s_window), (Layer *) s_normal_kg);

    // s_inv_selector
    s_inv_selector = inverter_layer_create(GRect(8, 88, 43, 35));
    layer_add_child(window_get_root_layer(s_window), (Layer *) s_inv_selector);

    // s_layer_1
    s_layer_1 = layer_create(GRect(7, 1, 134, 15));
    layer_add_child(window_get_root_layer(s_window), (Layer *) s_layer_1);
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
    inverter_layer_destroy(s_inv_timer);
    text_layer_destroy(s_normal_kg);
    inverter_layer_destroy(s_inv_selector);
    layer_destroy(s_layer_1);
}
// END AUTO-GENERATED UI CODE

static void load_machines_data() {
    Machine *m = first_machine;

    persist_read_string(DATA_MACHINES, data_buffer, 256);
    parsed *p = parsed_create(data_buffer, ';');

    while (m != NULL && !parsed_done(p)) {
        int mkey = parsed_number(p);
        if (mkey != m->mkey) {
            APP_LOG(APP_LOG_LEVEL_DEBUG, "Wrong mkey. Skipping data load.");
            free(p);
            break;
        }

        m->warmup_kg = parsed_number(p);
        m->normal_kg = parsed_number(p);
        m->set_1 = parsed_number(p);
        m->set_2 = parsed_number(p);
        m->set_3 = parsed_number(p);
        parsed_skip(p, 1);

        m = m->next;
    }
    free(p);
}

static void save_machines_data() {
    // "0;100;101;10;11;12;;1;102;103;13;14;15;;";
    data_buffer[0] = 0;

    char tmp[24];

    Machine *m = first_machine;
    while (m != NULL) {
        snprintf(tmp, 24, "%d;%d;%d;%d;%d;%d;;", m->mkey, m->warmup_kg, m->normal_kg, m->set_1, m->set_2, m->set_3);
//        APP_LOG(APP_LOG_LEVEL_DEBUG, "appending: --%s--", tmp);
        strcat(data_buffer, tmp);
        m = m->next;
    }
    if (strlen(data_buffer) > 255) {
        APP_LOG(APP_LOG_LEVEL_ERROR, "data is too big. needs truncation.");
        return;
    }
    // TODO: save
//    APP_LOG(APP_LOG_LEVEL_DEBUG, "saved data: --%s--", data_buffer);

    persist_write_string(DATA_MACHINES, data_buffer);
}

static void handle_window_unload(Window *window) {
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

    layer_mark_dirty(s_layer_1);
}

static void update_inv_layer() {
    GRect rect = layer_get_frame(editable_fields[current_field]);
    layer_set_frame((Layer *) s_inv_selector, rect);
}

static void decrease_click_handler(ClickRecognizerRef recognizer, void *context) {
    switch (current_field) {
        case F_TITLE:
            if (current_machine->next != NULL) {
                current_machine = current_machine->next;
                update_machine();
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
    update_machine();
}

static void increase_click_handler(ClickRecognizerRef recognizer, void *context) {
    switch (current_field) {
        case F_TITLE:
            if (current_machine->prev != NULL) {
                current_machine = current_machine->prev;
                update_machine();
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
    update_machine();
}

static void prev_click_handler(ClickRecognizerRef recognizer, void *context) {
    if (current_field > 0) {
        current_field--;
        update_inv_layer();
    }
}

static void next_click_handler(ClickRecognizerRef recognizer, void *context) {
    if (current_field == F__COUNT - 1) {
        current_machine->is_done = true;
        current_field = 0;
    } else {
        current_field++;
    }

    update_inv_layer();
    save_machines_data();
}

static void click_config_provider(void *context) {
    window_single_click_subscribe(BUTTON_ID_SELECT, next_click_handler);
    window_single_click_subscribe(BUTTON_ID_BACK, prev_click_handler);

    window_single_repeating_click_subscribe(BUTTON_ID_UP, 100, increase_click_handler);
    window_single_repeating_click_subscribe(BUTTON_ID_DOWN, 100, decrease_click_handler);
}

static void create_all_machines() {

    Machine *last_created_machine = NULL;

    for (int i = M_WARMUP; i <= M_COOLDOWN; i++) {
        Machine *m = malloc(sizeof(Machine));
        m->mkey = i;
        m->next = NULL;
        m->prev = last_created_machine;
        m->warmup_kg = 0;
        m->normal_kg = 0;
        m->set_1 = 0;
        m->set_2 = 0;
        m->set_3 = 0;
        m->warmup_kg_str = malloc(4);
        m->normal_kg_str = malloc(4);
        m->set_1_str = malloc(4);
        m->set_2_str = malloc(4);
        m->set_3_str = malloc(4);
        m->is_done = false;

        switch (i) {
            case M_WARMUP:
                m->title = "Warmup (run)";
                break;
            case M_SHOULDERS:
                m->title = "Shoulders";
                break;
            case M_CHEST:
                m->title = "Chest";
                break;
            case M_TRICEPS:
                m->title = "Triceps";
                break;
            case M_BICEPS:
                m->title = "Biceps";
                break;
            case M_UPPER_BACK:
                m->title = "Back (upper!)";
                break;
            case M_LOWER_BACK:
                m->title = "Back (lower)";
                break;
            case M_LEGS_ULTIMATE:
                m->title = "Legs (ultimate)";
                break;
            case M_ABS:
                m->title = "ABS (front)";
                break;
            case M_ABS_SIDE:
                m->title = "ABS (sides)";
                break;
            case M_COOLDOWN:
                m->title = "Cooldown (cycle)";
                break;
            default:
                APP_LOG(APP_LOG_LEVEL_DEBUG, "!!!!!!!!!! UNKNOWN MACHINE: %d", i);
        }

        if (first_machine == NULL) {
            first_machine = m;
        }
        if (last_created_machine != NULL) {
            last_created_machine->next = m;
        }
        last_created_machine = m;
    }
}

void draw_rectangles(struct Layer *layer, GContext *ctx) {
    int16_t left_pos = 4;
    int16_t top_pos = 3;
    Machine *m = first_machine;
    while (m != NULL) {
        GRect rect;
        if (current_machine == m) {
            rect = (GRect) {.origin = {left_pos - 4, top_pos - 3}, .size = {9 + 8, 9 + 6}};
            rect = (GRect) {.origin = {left_pos - 3, top_pos - 2}, .size = {9 + 6, 9 + 4}};
        } else {
            rect = (GRect) {.origin = {left_pos, top_pos}, .size = {9, 9}};
        }
        if (m->is_done) {
            graphics_fill_rect(ctx, rect, 0, GCornerNone);
        } else {
            graphics_draw_rect(ctx, rect);
        }

        left_pos += 11;
        m = m->next;
    }
}

void show_window_with_timer(void) {
    initialise_ui();
    window_set_window_handlers(s_window, (WindowHandlers) {
            .unload = handle_window_unload,
    });

    window_set_click_config_provider(s_window, click_config_provider);
    layer_set_update_proc(s_layer_1, draw_rectangles);

    editable_fields[F_TITLE] = (Layer *) s_machine;
    editable_fields[F_WARMUP_KG] = (Layer *) s_warmup_kg;
    editable_fields[F_NORMAL_KG] = (Layer *) s_normal_kg;
    editable_fields[F_SET_1] = (Layer *) s_set_1;
    editable_fields[F_SET_2] = (Layer *) s_set_2;
    editable_fields[F_SET_3] = (Layer *) s_set_3;

    current_field = F_SET_1;

    create_all_machines();
    load_machines_data();

    current_machine = first_machine;

    update_machine();
    update_inv_layer();

    window_stack_push(s_window, true);

//    s_invert_all = inverter_layer_create(layer_get_bounds(window_get_root_layer(s_window)));
//    layer_add_child(window_get_root_layer(s_window), (Layer *) s_invert_all);
}

void hide_window_with_timer(void) {
    window_stack_remove(s_window, true);
}
