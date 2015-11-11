#include "pebble.h"
#include "window_with_timer.h"
#include "window_location.h"
#include "window_menu.h"
#include "../ui_prepare/window_prepare.h"
#include "../ui_upload/window_upload.h"
#include "../ui/window_error.h"
#include "../data/machine.h"
#include "../ui_delete/window_delete.h"

static Window *window;
static MenuLayer *menu_layer;

char *upload_status_str = "? workout(s) to upload";
// TODO: only delete workouts that were uploaded
char *delete_status_str = "? workout(s) to delete";

/*typedef enum {
    MENUITEM_START,
    MENUITEM_END,
    MENUITEM_CANCEL,
    MENUITEM_PREPARE,
    MENUITEM_UPLOAD
} MenuItemType;

typedef struct {
    char* title;
    char* subtitle;
    MenuItemType type;
} MenuItemDefinition;

typedef struct MenuItem MenuItem;
struct MenuItem {
    MenuItemDefinition* def;
    MenuItem next;
    bool is_last;
};

*//*typedef struct {
    MenuItem* first_item;
} Menu;*//*

MenuItemDefinition start = (MenuItemDefinition) {
        .title = "Start",
        .type = MENUITEM_START,
        .subtitle = NULL
};
MenuItemDefinition cancel = (MenuItemDefinition) {
        .title = "Cancel",
        .type = MENUITEM_CANCEL,
        .subtitle = NULL
};

MenuItem not_active = (MenuItem) {
        .def = &start,
        .is_last = false,
        .next = (MenuItem) {
                .def = &cancel,
                .is_last = true
        }
};

MenuItem* gym_menu = &not_active;*/

typedef enum {
    STATE_NOT_ACTIVE,
    STATE_ACTIVE,
    STATE_FINISHED
} WorkoutState;

WorkoutState workout_state = STATE_NOT_ACTIVE;

void check_workout_state(Window *window);

static uint16_t menu_get_num_sections_callback(MenuLayer *menu_layer, void *data) {
    return 2;
}

static uint16_t menu_get_num_rows_callback(MenuLayer *menu_layer, uint16_t section_index, void *data) {
    switch (section_index) {
        case 0:
            if (workout_state == STATE_NOT_ACTIVE) {
                return 1;
            } else {
                return 3;
            }

        case 1:
            return 3;

        default:
            return 0;
    }
}

static int16_t menu_get_header_height_callback(MenuLayer *menu_layer, uint16_t section_index, void *data) {
    return MENU_CELL_BASIC_HEADER_HEIGHT;
}

static int16_t menu_get_cell_height_callback(MenuLayer *menu_layer, MenuIndex *menu_index, void *data) {
    if ((menu_index->section == 0) ||
            (menu_index->section == 1 && menu_index->row == 0)) {
        return 30;
    }

    return 44;
}


// Here we draw what each header is
static void menu_draw_header_callback(GContext *ctx, const Layer *cell_layer, uint16_t section_index, void *data) {
    // Determine which section we're working with
    switch (section_index) {
        case 0:
            // Draw title text in the section header
            menu_cell_basic_header_draw(ctx, cell_layer, "At The Gym");
            break;

        case 1:
            menu_cell_basic_header_draw(ctx, cell_layer, "At Home");
            break;
        default:
            break;
    }
}

// This is the menu item draw callback where you specify what each item should look like
static void menu_draw_row_callback(GContext *ctx, const Layer *cell_layer, MenuIndex *cell_index, void *data) {
    // Determine which section we're going to draw in
    switch (cell_index->section) {
        case 0:
            switch (workout_state) {
                case STATE_NOT_ACTIVE:
                    switch (cell_index->row) {
                        case 0:
                            menu_cell_title_draw(ctx, cell_layer, "Start New");
                            break;
                        default:
                            break;
                    }
                    break;
                case STATE_ACTIVE:
                    switch (cell_index->row) {
                        case 0:
                            menu_cell_title_draw(ctx, cell_layer, "Continue");
                            break;

                        case 1:
                            menu_cell_title_draw(ctx, cell_layer, "End (Save)");
                            break;

                        case 2:
                            menu_cell_title_draw(ctx, cell_layer, "Cancel");
                            break;
                        default:
                            break;
                    }
                    break;
                case STATE_FINISHED:
                    switch (cell_index->row) {
                        case 0:
                            menu_cell_title_draw(ctx, cell_layer, "View");
                            break;

                        case 1:
                            menu_cell_title_draw(ctx, cell_layer, "End (Save)");
                            break;

                        case 2:
                            menu_cell_title_draw(ctx, cell_layer, "Cancel");
                            break;
                        default:
                            break;
                    }
                    break;

            }
            break;

        case 1:
            switch (cell_index->row) {
                case 0:
                    // There is title draw for something more simple than a basic menu item
                    menu_cell_title_draw(ctx, cell_layer, "Prepare");
                    break;

                case 1:
                    menu_cell_basic_draw(ctx, cell_layer, "Upload workouts", upload_status_str, NULL);
                    break;

                case 2:
                    menu_cell_basic_draw(ctx, cell_layer, "Delete workouts", delete_status_str, NULL);
                    break;
                default:
                    break;
            }
        default:
            break;
    }
}

// Here we capture when a user selects a menu item
void menu_select_callback(MenuLayer *menu_layer, MenuIndex *cell_index, void *data) {
    if (workout_state == STATE_NOT_ACTIVE && cell_index->section == 0 && cell_index->row == 0) { // Start
        show_window_location();
        return;
    }
    if ((workout_state == STATE_ACTIVE && cell_index->section == 0 && cell_index->row == 0) || // Continue
            (workout_state == STATE_FINISHED && cell_index->section == 0 && cell_index->row == 0)) { // View
        show_window_with_timer(false, 0);
        return;
    }
    if ((workout_state == STATE_ACTIVE && cell_index->section == 0 && cell_index->row == 1) || // End (Save)
            (workout_state == STATE_FINISHED && cell_index->section == 0 && cell_index->row == 1)) { // End (Save)
        bool success = workout_end_current();
        if (!success) {
            show_window_error("All save slots are taken. Please upload some of workouts to continue.");
            return;
        }
        check_workout_state(NULL);
        return;
    }
    if ((workout_state == STATE_ACTIVE && cell_index->section == 0 && cell_index->row == 2) ||
            (workout_state == STATE_FINISHED && cell_index->section == 0 && cell_index->row == 2)) { // Cancel
        workout_cancel_current();
        check_workout_state(NULL);

        menu_layer_set_selected_index(menu_layer, (MenuIndex) {.row = 0, .section = 0}, MenuRowAlignNone, false);
        return;
    }
    if (cell_index->section == 1 && cell_index->row == 0) {
        show_window_prepare();
        return;
    }
    if (cell_index->section == 1 && cell_index->row == 1) {
        show_window_upload();
        return;
    }
    if (cell_index->section == 1 && cell_index->row == 2) {
        show_window_delete();
        return;
    }
}

void check_workout_state(Window *window) {
    Workout *w = workout_create_without_machines();
    workout_load_current_without_machines(w);

    workout_state = STATE_NOT_ACTIVE;
    if (w->time_start != 0) workout_state = STATE_ACTIVE;
    if (w->time_end != 0) workout_state = STATE_FINISHED;

    SaveState state = slots_load_state();
    uint8_t slots_taken = (uint8_t) ((state.save1_in_use ? 1 : 0) + (state.save2_in_use ? 1 : 0) + (state.save3_in_use ? 1 : 0));
    upload_status_str[0] = (char) ('0' + slots_taken);
    delete_status_str[0] = (char) ('0' + slots_taken);

    menu_layer_reload_data(menu_layer);

    workout_destroy(w);
}

// This initializes the menu upon window load
void window_menu_load(Window *window) {
    Layer *window_layer = window_get_root_layer(window);

    menu_layer = menu_layer_create(layer_get_frame(window_layer));
    menu_layer_set_callbacks(menu_layer, NULL, (MenuLayerCallbacks) {
            .get_num_sections = menu_get_num_sections_callback,
            .get_num_rows = menu_get_num_rows_callback,
            .get_header_height = menu_get_header_height_callback,
            .get_cell_height = menu_get_cell_height_callback,
            .draw_header = menu_draw_header_callback,
            .draw_row = menu_draw_row_callback,
            .select_click = menu_select_callback,
    });
    menu_layer_set_click_config_onto_window(menu_layer, window);

    layer_add_child(window_layer, menu_layer_get_layer(menu_layer));
}

void window_menu_unload(Window *window) {
    menu_layer_destroy(menu_layer);
}

void show_window_menu(void) {
    window = window_create();

    // Setup the window handlers
    window_set_window_handlers(window, (WindowHandlers) {
            .load = window_menu_load,
            .unload = window_menu_unload,
            .appear = check_workout_state
    });

    window_stack_push(window, true /* Animated */);
}
