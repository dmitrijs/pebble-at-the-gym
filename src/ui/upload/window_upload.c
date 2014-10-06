#include "window_upload.h"
#include "../../data/machine.h"
#include "../../lib/message-queue.h"

typedef Layer ProgressBarLayer;

static ProgressBarLayer *progress_bar_layer_1;
static ProgressBarLayer *progress_bar_layer_2;
static ProgressBarLayer *progress_bar_layer_3;

typedef struct {
    int16_t progress;
    int16_t maximum;
} ProgressData;

static bool upload_state[3][12];

// BEGIN AUTO-GENERATED UI CODE; DO NOT MODIFY
static Window *s_window;
static GFont s_res_gothic_18_bold;
static TextLayer *s_textlayer_1;
static TextLayer *s_textlayer_2;
static TextLayer *s_textlayer_3;
static TextLayer *s_textlayer_4;

static void initialise_ui(void) {
    s_window = window_create();
    window_set_fullscreen(s_window, false);

    s_res_gothic_18_bold = fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD);
    // s_textlayer_1
    s_textlayer_1 = text_layer_create(GRect(2, 1, 141, 40));
    text_layer_set_text(s_textlayer_1, "Uploading workouts,\nplease wait...");
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
    window_destroy(s_window);
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

    mqueue_destroy();
}

static char buf[201];
static char group_str[2] = "?";
static char operation_str[3] = "m?";

void slot_data_received(int index, Layer *bar, char *operation, char *data) {
    APP_LOG(APP_LOG_LEVEL_ERROR, "slot %d data received", index);
    if (operation[0] == 'w') {
        upload_state[index][0] = true;
    }
    if (operation[0] == 'm') {
        upload_state[index][(operation[1] - 'A' + 1)] = true;
    }
    uint16_t progress = 0;
    for (int i = 0; i < 12; i++) {
        progress += (upload_state[index][i] ? 1 : 0);
    }
    ProgressData *bar_data = (ProgressData *) layer_get_data(bar);
    bar_data->progress = progress;
    layer_mark_dirty(bar);
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

void start_upload_by_data_position(uint32_t index, uint32_t data_position) {
    group_str[0] = (char) ('0' + index);

    Workout *w = workout_create();
    workout_load_by_data_position(w, data_position);

    uint32_t i = 0;

    workout_serialize(buf, w);
    mqueue_add(group_str, "w", buf);
    i++;

    Machine *m = w->first_machine;
    while (m != NULL) {
        machine_serialize(buf, m);
        operation_str[1] = (char) ('A' + m->mkey);
        mqueue_add(group_str, operation_str, buf);

        m = m->next;
        i++;
    }

    workout_destroy(w);
}

void start_upload() {
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 12; ++j) {
            upload_state[i][j] = false;
        }
    }

    SaveState state = slots_load_state();
    if (state.save2_in_use) {
        start_upload_by_data_position(1, DATA_WORKOUT_SAVE_2);
    } else {
        layer_remove_from_parent(text_layer_get_layer(s_textlayer_3));
        layer_remove_from_parent(progress_bar_layer_2);
    }
    if (state.save3_in_use) {
        start_upload_by_data_position(2, DATA_WORKOUT_SAVE_3);
    } else {
        layer_remove_from_parent(text_layer_get_layer(s_textlayer_4));
        layer_remove_from_parent(progress_bar_layer_3);
    }
    if (state.save1_in_use) {
        start_upload_by_data_position(0, DATA_WORKOUT_SAVE_1);
    } else {
        layer_remove_from_parent(text_layer_get_layer(s_textlayer_2));
        layer_remove_from_parent(progress_bar_layer_1);
    }
}

void on_communication_ready() {
    start_upload();
}

void show_window_upload(void) {
    initialise_ui();
    progress_bar_layer_1 = progress_bar_layer_create(GRect(6, 64, 130, 10), (ProgressData) {.progress = 0, .maximum = 12});
    progress_bar_layer_2 = progress_bar_layer_create(GRect(6, 97, 130, 10), (ProgressData) {.progress = 0, .maximum = 12});
    progress_bar_layer_3 = progress_bar_layer_create(GRect(6, 132, 130, 10), (ProgressData) {.progress = 0, .maximum = 12});
    layer_add_child(window_get_root_layer(s_window), progress_bar_layer_1);
    layer_add_child(window_get_root_layer(s_window), progress_bar_layer_2);
    layer_add_child(window_get_root_layer(s_window), progress_bar_layer_3);

    window_set_window_handlers(s_window, (WindowHandlers) {
            .unload = handle_window_unload,
    });
    window_stack_push(s_window, true);

    mqueue_init(on_communication_ready);
    mqueue_register_handler("0", slot_0_data_received);
    mqueue_register_handler("1", slot_1_data_received);
    mqueue_register_handler("2", slot_2_data_received);
}

void hide_window_upload(void) {
    window_stack_remove(s_window, true);
}
