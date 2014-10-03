#include <pebble.h>
#include "machine.h"
#include "../lib/parsed.h"
#include "../lib/key_value_unsafe.h"
#include "../lib/persistor.h"

char data_buffer[300];

Machine *machines_create_all();

void machines_destroy(Machine *first_machine);

void machines_data_save(Machine *first_machine);

void machines_data_load(Machine *first_machine, char *data);

Machine *machine_get_by_index(Workout *w, int index) {
    Machine *m = w->first_machine;
    while (m != NULL && index != 0) {
        m = m->next;
        index--;
    }
    if (index == 0) {
        return m;
    }
    return NULL;
}

void machine_data_load(Machine *m, char *data) {
    parsed *p = parsed_create(data, '.');

    int mkey = parsed_number(p);
    if (mkey != m->mkey) {
        APP_LOG(APP_LOG_LEVEL_DEBUG, "Wrong mkey. Skipping data load.");
        return;
    }

    m->warmup_kg = parsed_number(p);
    m->normal_kg = parsed_number(p);
    m->set_1 = parsed_number(p);
    m->set_2 = parsed_number(p);
    m->set_3 = parsed_number(p);
    m->is_done = parsed_number(p) == 1;
    m->time_done = (uint16_t) parsed_number(p);

    free(p);
}

void workout_save_current(Workout *w) {
    size_t s_size = 512;
    char *res = malloc(s_size);
    res[0] = '\0';
    res = add_key_value_int_unsafe(res, s_size, "wl", w->location);
    res = add_key_value_int_unsafe(res, s_size, "ws", w->time_start);
    res = add_key_value_int_unsafe(res, s_size, "we", w->time_end);

    // "0;100;101;10;11;12;;1;102;103;13;14;15;;";
    data_buffer[0] = 0;

    char tmp[200];
    char m_key_str[3] = "m?";

    Machine *m = w->first_machine;
    while (m != NULL) {
        snprintf(tmp, 200, "%d.%d.%d.%d.%d.%d.%d.%d.", m->mkey, m->warmup_kg, m->normal_kg, m->set_1, m->set_2, m->set_3, m->is_done ? 1 : 0, (int) m->time_done);
        m_key_str[1] = (char) ('A' + m->mkey);
        res = add_key_value_unsafe(res, s_size, m_key_str, tmp);

        m = m->next;
    }

    if ((int) strlen(res) * 1.5 > s_size) {
        APP_LOG(APP_LOG_LEVEL_WARNING, "DATA SIZE APPROACHES S_SIZE VALUE!");
    }

    persist_write_long_string(DATA_WORKOUT_CURRENT, res);
}

bool workout_try_backup(Workout *w) {
    return false;
}

void read_data_callback(void *ctx, char *key, char *value) {
    Workout *workout = (Workout *) ctx;

    APP_LOG(APP_LOG_LEVEL_DEBUG, "key -> val: %s -> %s", key, value);

    switch (key[0]) {
        case 'm': {
            Machine *pMachine = machine_get_by_index(workout, (key[1] - 'A'));
            if (pMachine == NULL) {
                APP_LOG(APP_LOG_LEVEL_DEBUG, "invalid machine index: %d", (key[1] - 'A'));
                break;
            }
            machine_data_load(pMachine, value);
        }
            break;
        case 'w': {
            switch (key[1]) {
                case 'l':
                    workout->location = atoi(value);
                    break;
                case 's':
                    workout->time_start = (uint16_t) atol(value);
                    break;
                case 'e':
                    workout->time_end = (uint16_t) atol(value);
                    break;
                default:
                    break;
            }
        }
        default:
            break;
    }
}

void workout_load_current(Workout *workout) {
    size_t len = persist_read_long_string_length(DATA_WORKOUT_CURRENT);
    APP_LOG(APP_LOG_LEVEL_DEBUG, "workout data len = %d", len);

    char *data = malloc(len);
    bool success = persist_read_long_string(DATA_WORKOUT_CURRENT, data);
    if (!success) {
        free(data);
        return;
    }

    read_key_values_unsafe(workout, data, read_data_callback);

    free(data);
}

void machines_data_load(Machine *first_machine, char *data) {
    Machine *m = first_machine;

    // persist_read_string(DATA_WORKOUT_CURRENT, data_buffer, 256);
    parsed *p = parsed_create(data, '.');

    while (m != NULL && !parsed_done(p)) {
        int mkey = parsed_number(p);
        if (mkey != m->mkey) {
            APP_LOG(APP_LOG_LEVEL_DEBUG, "Wrong mkey. Skipping data load.");
            break;
        }

        m->warmup_kg = parsed_number(p);
        m->normal_kg = parsed_number(p);
        m->set_1 = parsed_number(p);
        m->set_2 = parsed_number(p);
        m->set_3 = parsed_number(p);
        m->set_3 = parsed_number(p);
        m->is_done = parsed_number(p) == 1;
        m->time_done = (uint16_t) parsed_number(p);

        m = m->next;
    }
    free(p);
}

void machines_destroy(Machine *first_machine) {
    Machine *m = first_machine;

    while (m != NULL) {
        free(m->warmup_kg_str);
        free(m->normal_kg_str);
        free(m->set_1_str);
        free(m->set_2_str);
        free(m->set_3_str);

        Machine *next = m->next;
        free(m);

        m = next;
    }
}

void workout_destroy(Workout *w) {
    machines_destroy(w->first_machine);
    free(w);
}


Workout *workout_create() {
    Workout *w = malloc(sizeof(Workout));
    w->time_start = 0;
    w->time_end = 0;
    w->location = -1;
    w->first_machine = machines_create_all();
    return w;
}

Machine *machines_create_all() {

    Machine *first_machine = NULL;
    Machine *last_created_machine = NULL;

    for (int i = M_WARMUP; i < M__COUNT; i++) {
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
        m->time_done = 0;

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
    return first_machine;
}
