#include "window_upload.h"
#include "../data/machine.h"
#include "../lib/message-queue.h"

typedef Layer ProgressBarLayer;

static ProgressBarLayer *progress_bar_layer_1;
static ProgressBarLayer *progress_bar_layer_2;
static ProgressBarLayer *progress_bar_layer_3;

typedef struct {
    int16_t progress;
    int16_t maximum;
} ProgressData;

// BEGIN AUTO-GENERATED UI CODE; DO NOT MODIFY
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
    text_layer_set_text(s_textlayer_1, "Press 'select' to start the upload.");
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
    //window_destroy(s_window);
    text_layer_destroy(s_textlayer_1);
    text_layer_destroy(s_textlayer_2);
    text_layer_destroy(s_textlayer_3);
    text_layer_destroy(s_textlayer_4);
}
// END AUTO-GENERATED UI CODE

static void progress_bar_layer_update(ProgressBarLayer *bar, GContext *ctx) {
    ProgressData *data = (ProgressData *) layer_get_data(bar);

    GRect bounds = layer_get_bounds(bar);
    int16_t width_progress = (bounds.size.w * data->progress) / data->maximum;

    // Outline the progress bar
    graphics_context_set_stroke_color(ctx, GColorBlack);
    graphics_draw_round_rect(ctx, GRect(0, 0, bounds.size.w, bounds.size.h), 4);

    // Fill the progress bar
    graphics_context_set_fill_color(ctx, GColorBlack);
    graphics_fill_rect(ctx, GRect(0, 0, width_progress, bounds.size.h), 4, GCornersAll);
}

static ProgressBarLayer *progress_bar_layer_create(GRect bounds, ProgressData initial_data) {
    ProgressBarLayer *progress_bar_layer = layer_create_with_data(bounds, sizeof(ProgressData));
    ProgressData *data = (ProgressData *) layer_get_data(progress_bar_layer);
    data->progress = initial_data.progress;
    data->maximum = initial_data.maximum;
    layer_set_update_proc(progress_bar_layer, progress_bar_layer_update);
    layer_mark_dirty(progress_bar_layer);

    return progress_bar_layer;
}

static void progress_bar_destroy(ProgressBarLayer *progress_bar_layer) {
    layer_destroy(progress_bar_layer);
}

static void handle_window_unload(Window *window) {
    destroy_ui();

    progress_bar_destroy(progress_bar_layer_1);
    progress_bar_destroy(progress_bar_layer_2);
    progress_bar_destroy(progress_bar_layer_3);

    // TODO: maybe implement?, then remove "deregister_callbacks" method
    //mqueue_destroy();
}

static char buf[201];
static char group_str[2] = "?";
static char operation_str[3] = "m?";
static bool upload_in_progress[3];

void slot_data_received(int index, Layer *bar, char *operation, char *data) {
    APP_LOG(APP_LOG_LEVEL_INFO, "slot %d data received", index);

    ProgressData *bar_data = (ProgressData *) layer_get_data(bar);
    bar_data->progress = bar_data->maximum;
    layer_mark_dirty(bar);

    upload_in_progress[index] = false;

    if (!upload_in_progress[0] && !upload_in_progress[1] && !upload_in_progress[2]) {
        hide_window_upload();
    }
}

void slot_0_data_received(char *operation, char *data) {
    slot_data_received(0, progress_bar_layer_1, operation, data);
}

void slot_1_data_received(char *operation, char *data) {
    slot_data_received(1, progress_bar_layer_2, operation, data);
}

void slot_2_data_received(char *operation, char *data) {
    slot_data_received(2, progress_bar_layer_3, operation, data);
}

void prepare_upload_by_data_position(uint32_t index, uint32_t data_position) {
    group_str[0] = (char) ('0' + index);

    Workout *w = workout_create();
    workout_load_by_data_position(w, data_position);

    uint32_t i = 0;

    char bigbuf[1600];

    workout_serialize(bigbuf, w);
    APP_LOG(APP_LOG_LEVEL_INFO, "concat length: %d", strlen(bigbuf));
    i++;

    Machine *m = w->first_machine;
    while (m != NULL) {
        machine_serialize(buf, m);
        operation_str[1] = (char) ('A' + m->mkey);
        operation_str[2] = 0;
        buf[200] = 0;

        strcat(bigbuf, "\n");
        strcat(bigbuf, buf);
        APP_LOG(APP_LOG_LEVEL_INFO, "concat length: %d", strlen(bigbuf));

        m = m->next;
        i++;
    }

    mqueue_add(group_str, "w", bigbuf);

    workout_destroy(w);
}

void prepare_upload() {
    for (int i = 0; i < 3; ++i) {
        upload_in_progress[i] = true;
    }

    SaveState state = slots_load_state();
    if (state.save1_in_use) {
        prepare_upload_by_data_position(0, DATA_WORKOUT_SAVE_1);
    } else {
        upload_in_progress[0] = false;
    }
    if (state.save2_in_use) {
        prepare_upload_by_data_position(1, DATA_WORKOUT_SAVE_2);
    } else {
        upload_in_progress[1] = false;
    }
    if (state.save3_in_use) {
        prepare_upload_by_data_position(2, DATA_WORKOUT_SAVE_3);
    } else {
        upload_in_progress[2] = false;
    }
}

void dump(uint32_t pos) {
    char slots[30];

    int i = 0;
    for (;i <= 15; i++) {
        if (persist_exists(pos + i)) {
            slots[i] = 'X';
        } else {
            slots[i] = '-';
        }
    }
    slots[i] = 0;

    APP_LOG(APP_LOG_LEVEL_INFO, slots);

    char buf[300];

    i= 0;
    for (;i <= 15; i++) {
        buf[0] = 0;
        if (persist_exists(pos + i)) {
            APP_LOG(APP_LOG_LEVEL_INFO, "slot %d data:", i);
            persist_read_string(pos + i, buf, 250);
            APP_LOG(APP_LOG_LEVEL_INFO, buf);
        }
    }
}

void initialize_progress_bars() {
    progress_bar_layer_1 = progress_bar_layer_create(GRect(6, 64, 130, 10), (ProgressData) {.progress = 0, .maximum = M__COUNT + 1});
    progress_bar_layer_2 = progress_bar_layer_create(GRect(6, 97, 130, 10), (ProgressData) {.progress = 0, .maximum = M__COUNT + 1});
    progress_bar_layer_3 = progress_bar_layer_create(GRect(6, 132, 130, 10), (ProgressData) {.progress = 0, .maximum = M__COUNT + 1});

    SaveState state = slots_load_state();
    if (state.save1_in_use) {
        layer_add_child(window_get_root_layer(s_window), progress_bar_layer_1);
    } else {
        layer_remove_from_parent(text_layer_get_layer(s_textlayer_2));
    }
    if (state.save2_in_use) {
        layer_add_child(window_get_root_layer(s_window), progress_bar_layer_2);
    } else {
        layer_remove_from_parent(text_layer_get_layer(s_textlayer_3));
    }
    if (state.save3_in_use) {
        layer_add_child(window_get_root_layer(s_window), progress_bar_layer_3);
    } else {
        layer_remove_from_parent(text_layer_get_layer(s_textlayer_4));
    }

    // print persistence state:
    if (persist_exists(DATA_WORKOUT_CURRENT)) {
        APP_LOG(APP_LOG_LEVEL_INFO, "current exists:");

        dump(DATA_WORKOUT_CURRENT);
    } else {
        APP_LOG(APP_LOG_LEVEL_INFO, "no current");
    }
    if (persist_exists(DATA_WORKOUT_SAVE_1)) {
        APP_LOG(APP_LOG_LEVEL_INFO, "save1 exists:");

        dump(DATA_WORKOUT_SAVE_1);
    } else {
        APP_LOG(APP_LOG_LEVEL_INFO, "no save1");
    }
    if (persist_exists(DATA_WORKOUT_SAVE_2)) {
        APP_LOG(APP_LOG_LEVEL_INFO, "save2 exists:");

        dump(DATA_WORKOUT_SAVE_2);
    } else {
        APP_LOG(APP_LOG_LEVEL_INFO, "no save2");
    }
}

static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
    text_layer_set_text(s_textlayer_1, "Uploading workouts,\nplease wait...");

    mqueue_deregister_handlers();
    mqueue_register_handler("0", slot_0_data_received);
    mqueue_register_handler("1", slot_1_data_received);
    mqueue_register_handler("2", slot_2_data_received);
    mqueue_init(false);
    prepare_upload();
    mqueue_enable_sending();
}

static void prev_click_handler(ClickRecognizerRef recognizer, void *context) {
    hide_window_upload();
}

static void click_config_provider(void *context) {
    window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
    window_single_click_subscribe(BUTTON_ID_BACK, prev_click_handler);
}


void show_window_upload(void) {
    initialise_ui();
    initialize_progress_bars();

    window_set_click_config_provider(s_window, click_config_provider);

    window_set_window_handlers(s_window, (WindowHandlers) {
            .unload = handle_window_unload,
    });
    window_stack_push(s_window, true);
}

void hide_window_upload(void) {
    window_stack_remove(s_window, true);
}
