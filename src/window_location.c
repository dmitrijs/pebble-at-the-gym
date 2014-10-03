#include "pebble.h"
#include "window_location.h"
#include "window_with_timer.h"
#include "data/machine.h"
#include "window_menu.h"

static Window *window;
static MenuLayer *menu_layer;

static uint16_t menu_location_get_num_sections_callback(MenuLayer *menu_layer, void *data) {
    return 2;
}

static uint16_t menu_location_get_num_rows_callback(MenuLayer *menu_layer, uint16_t section_index, void *data) {
    switch (section_index) {
        case 0:
            return 7;

        case 1:
            return 1;

        default:
            return 0;
    }
}

static int16_t menu_location_get_header_height_callback(MenuLayer *menu_layer, uint16_t section_index, void *data) {
    return MENU_CELL_BASIC_HEADER_HEIGHT;
}

static int16_t menu_location_get_cell_height_callback(MenuLayer *menu_layer, MenuIndex *menu_index, void *data) {
    return 30;
}


// Here we draw what each header is
static void menu_location_draw_header_callback(GContext *ctx, const Layer *cell_layer, uint16_t section_index, void *data) {
    // Determine which section we're working with
    switch (section_index) {
        case 0:
            // Draw title text in the section header
            menu_cell_basic_header_draw(ctx, cell_layer, "CityFitness");
            break;

        case 1:
            menu_cell_basic_header_draw(ctx, cell_layer, "Mevi Gym");
            break;
    }
}

// This is the menu item draw callback where you specify what each item should look like
static void menu_location_draw_row_callback(GContext *ctx, const Layer *cell_layer, MenuIndex *cell_index, void *data) {
    // Determine which section we're going to draw in
    switch (cell_index->section) {
        case 0:
            // Use the row to specify which item we'll draw
            switch (cell_index->row) {
                case 0:
                    menu_cell_title_draw(ctx, cell_layer, "Agenskalns");
                    break;
                case 1:
                    menu_cell_title_draw(ctx, cell_layer, "Berga Bazars");
                    break;
                case 2:
                    menu_cell_title_draw(ctx, cell_layer, "Bolero (Spice)");
                    break;
                case 3:
                    menu_cell_title_draw(ctx, cell_layer, "Domina");
                    break;
                case 4:
                    menu_cell_title_draw(ctx, cell_layer, "Olimpia");
                    break;
                case 5:
                    menu_cell_title_draw(ctx, cell_layer, "Sky & More");
                    break;
                case 6:
                    menu_cell_title_draw(ctx, cell_layer, "Radisson BLU");
                    break;
            }
            break;

        case 1:
            switch (cell_index->row) {
                case 0:
                    // There is title draw for something more simple than a basic menu item
                    menu_cell_title_draw(ctx, cell_layer, "Mevi Gym");
                    break;
            }
    }
}

// Here we capture when a user selects a menu item
void menu_location_select_callback(MenuLayer *menu_layer, MenuIndex *cell_index, void *data) {

    hide_window_location();

    show_window_with_timer(true /* new workout */, cell_index->section * 10 + cell_index->row);
}

// This initializes the menu upon window load
void window_location_load(Window *window) {
    Layer *window_layer = window_get_root_layer(window);

    menu_layer = menu_layer_create(layer_get_frame(window_layer));
    menu_layer_set_callbacks(menu_layer, NULL, (MenuLayerCallbacks) {
            .get_num_sections = menu_location_get_num_sections_callback,
            .get_num_rows = menu_location_get_num_rows_callback,
            .get_header_height = menu_location_get_header_height_callback,
            .get_cell_height = menu_location_get_cell_height_callback,
            .draw_header = menu_location_draw_header_callback,
            .draw_row = menu_location_draw_row_callback,
            .select_click = menu_location_select_callback,
    });
    menu_layer_set_click_config_onto_window(menu_layer, window);

    layer_add_child(window_layer, menu_layer_get_layer(menu_layer));
}

void window_location_unload(Window *window) {
    menu_layer_destroy(menu_layer);
}

void show_window_location() {
    window = window_create();

    window_set_window_handlers(window, (WindowHandlers) {
            .load = window_location_load,
            .unload = window_location_unload,
    });

    window_stack_push(window, true /* Animated */);
}

void hide_window_location(void) {
    window_stack_remove(window, true);
}
