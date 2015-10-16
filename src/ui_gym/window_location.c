#include "pebble.h"
#include "window_location.h"
#include "window_with_timer.h"

static Window *window;
static MenuLayer *menu_layer;

#define LOCATION_COMPANIES 2

typedef struct {
    char code;
    char title[20];
} Location;

typedef struct {
    uint8_t locations_count;
    Location locations[20];
    char title[20];
} LocationCompany;

LocationCompany companies[LOCATION_COMPANIES];

static uint16_t menu_location_get_num_sections_callback(MenuLayer *menu_layer, void *data) {
    return 2;
}

static uint16_t menu_location_get_num_rows_callback(MenuLayer *menu_layer, uint16_t section_index, void *data) {
    switch (section_index) {
        case 0:
        case 1:
            return companies[section_index].locations_count;

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

static void menu_location_draw_header_callback(GContext *ctx, const Layer *cell_layer, uint16_t section_index, void *data) {
    switch (section_index) {
        case 0:
        case 1:
            menu_cell_basic_header_draw(ctx, cell_layer, companies[section_index].title);
            break;

        default:
            break;
    }
}

static void menu_location_draw_row_callback(GContext *ctx, const Layer *cell_layer, MenuIndex *cell_index, void *data) {
    if (cell_index->section >= LOCATION_COMPANIES ||
            cell_index->row >= companies[cell_index->section].locations_count) {
        menu_cell_title_draw(ctx, cell_layer, "invalid menu index");
        return;
    }
    menu_cell_title_draw(ctx, cell_layer, companies[cell_index->section].locations[cell_index->row].title);
}

void menu_location_select_callback(MenuLayer *menu_layer, MenuIndex *cell_index, void *data) {

    show_window_with_timer(true /* new workout */, companies[cell_index->section].locations[cell_index->row].code);

    hide_window_location();
}

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
    companies[0] = (LocationCompany) {.title = "CityFitness", .locations_count = 7};
    companies[0].locations[0] = (Location) {.code = 'A', .title = "Agenskalns"};
    companies[0].locations[1] = (Location) {.code = 'B', .title = "Berga Bazars"};
    companies[0].locations[2] = (Location) {.code = 'S', .title = "Spice (Bolero)"};
    companies[0].locations[3] = (Location) {.code = 'D', .title = "Domina"};
    companies[0].locations[4] = (Location) {.code = 'O', .title = "Olimpia"};
    companies[0].locations[5] = (Location) {.code = '&', .title = "Sky & More"};
    companies[0].locations[6] = (Location) {.code = 'R', .title = "Radisson BLU"};

    companies[1] = (LocationCompany) {.title = "Mevi Gym", .locations_count = 1};
    companies[1].locations[0] = (Location) {.code = 'M', .title = "Mevi Gym"};

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
