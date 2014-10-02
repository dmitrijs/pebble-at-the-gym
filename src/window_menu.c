#include "pebble.h"
#include "window_prepare.h"
#include "window_with_timer.h"

static Window *window;
static MenuLayer *menu_layer;

bool workout_is_active = false;

/*typedef struct {
    char* title;
    char* subtitle;
    int normal_kg;
    int set_1;
    int set_2;
    int set_3;

    char *warmup_kg_str;
    char *normal_kg_str;
    char *set_1_str;
    char *set_2_str;
    char *set_3_str;

    bool is_done;
    uint16_t time_done;
} MenuItemDefinition;

typedef struct MenuItem MenuItem;
struct MenuItem {
    MenuItemDefinition* def;
    MenuItem *next;
};

typedef struct {
    MenuItem* first_item;
} Menu;*/


static uint16_t menu_get_num_sections_callback(MenuLayer *menu_layer, void *data) {
    return 2;
}

static uint16_t menu_get_num_rows_callback(MenuLayer *menu_layer, uint16_t section_index, void *data) {
    switch (section_index) {
        case 0:
            if (workout_is_active) {
                return 2;
            } else {
                return 1;
            }

        case 1:
            return 2;

        default:
            return 0;
    }
}

static int16_t menu_get_header_height_callback(MenuLayer *menu_layer, uint16_t section_index, void *data) {
    return MENU_CELL_BASIC_HEADER_HEIGHT;
}

static int16_t menu_get_cell_height_callback(MenuLayer *menu_layer, MenuIndex *menu_index, void *data) {

    if ((!workout_is_active && menu_index->section == 0 && menu_index->row == 0) ||
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
    }
}

// This is the menu item draw callback where you specify what each item should look like
static void menu_draw_row_callback(GContext *ctx, const Layer *cell_layer, MenuIndex *cell_index, void *data) {
    // Determine which section we're going to draw in
    switch (cell_index->section) {
        case 0:
            // Use the row to specify which item we'll draw
            if (workout_is_active) {
                switch (cell_index->row) {
                    case 0:
                        menu_cell_basic_draw(ctx, cell_layer, "Continue", "Workout at Bolero", NULL);
                        break;

                    case 1:
                        menu_cell_basic_draw(ctx, cell_layer, "End", "Workout at Bolero", NULL);
                        break;
                }
            } else {
                switch (cell_index->row) {
                    case 0:
                        menu_cell_title_draw(ctx, cell_layer, "Start New");
                        break;
                }
            }
            break;

        case 1:
            switch (cell_index->row) {
                case 0:
                    // There is title draw for something more simple than a basic menu item
                    menu_cell_title_draw(ctx, cell_layer, "Prepare");
                    break;

                case 1:
                    menu_cell_basic_draw(ctx, cell_layer, "Upload workouts", "0 workouts to upload", NULL);
                    break;
            }
    }
}

// Here we capture when a user selects a menu item
void menu_select_callback(MenuLayer *menu_layer, MenuIndex *cell_index, void *data) {
    if (!workout_is_active && cell_index->section == 0 && cell_index->row == 0) {
        show_window_with_timer();
        return;
    }
    if (cell_index->section == 1 && cell_index->row == 0) {
        show_window_prepare();
        return;
    }
    // Use the row to specify which item will receive the select action
    switch (cell_index->row) {
        // This is the menu item with the cycling icon
        case 1:
            // Cycle the icon
//            current_icon = (current_icon + 1) % NUM_MENU_ICONS;
            // After changing the icon, mark the layer to have it updated
            layer_mark_dirty(menu_layer_get_layer(menu_layer));
            break;
    }

}

// This initializes the menu upon window load
void window_load(Window *window) {
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

void window_unload(Window *window) {
    // Destroy the menu layer
    menu_layer_destroy(menu_layer);

    // Cleanup the menu icons
//    for (int i = 0; i < NUM_MENU_ICONS; i++) {
//        gbitmap_destroy(menu_icons[i]);
//    }

    // And cleanup the background
//    gbitmap_destroy(menu_background);
}

void show_window_menu(void) {
    window = window_create();

    // Setup the window handlers
    window_set_window_handlers(window, (WindowHandlers) {
            .load = window_load,
            .unload = window_unload,
    });

    window_stack_push(window, true /* Animated */);
}

void hide_window_menu(void) {
    window_stack_remove(window, true);
}
