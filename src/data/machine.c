#include <pebble.h>
#include "machine.h"
#include "../lib/parsed.h"
#include "../lib/key_value_unsafe.h"

char res[201];
char tmp[201];
char tmp2[201];

Machine *machines_create_all();

void machines_destroy(Machine *first_machine);

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
    m->time_done = parsed_number_long(p);

    APP_LOG(APP_LOG_LEVEL_DEBUG, "Machine %d was initialized.", mkey);

    free(p);
}

void machine_save(Machine *m) {
    static char m_key_str[3] = "m?";

    snprintf(tmp, 200, "%d.%d.%d.", m->mkey, m->warmup_kg, m->normal_kg);
    snprintf(tmp2, 200, "%d.%d.%d.%d.%ld.", m->set_1, m->set_2, m->set_3, (m->is_done ? 1 : 0), m->time_done);
    strcat(tmp, tmp2);

    m_key_str[1] = (char) ('A' + m->mkey);

    snprintf(res, 200, "%s=%s;", m_key_str, tmp);

    persist_write_string((DATA_WORKOUT_CURRENT + 1 + m->mkey), res);
    APP_LOG(APP_LOG_LEVEL_WARNING, "machine saved = %s", res);
}

void workout_save_current(Workout *w) {
    snprintf(res, 200, "wl=%d;ws=%ld;we=%ld;", w->location, w->time_start, w->time_end);
    APP_LOG(APP_LOG_LEVEL_WARNING, "res: %s", res);
    persist_write_string(DATA_WORKOUT_CURRENT, res);
}

bool workout_try_backup(Workout *w) {
    return false;
}

void read_workout_data_callback(void *ctx, char *key, char *value) {
    Workout *workout = (Workout *) ctx;

    APP_LOG(APP_LOG_LEVEL_DEBUG, "workout key -> val: %s -> %s", key, value);

    switch (key[0]) {
        case 'w': {
            switch (key[1]) {
                case 'l':
                    workout->location = atoi(value);
                    break;
                case 's':
                    workout->time_start = atol(value);
                    break;
                case 'e':
                    workout->time_end = atol(value);
                    break;
                default:
                    break;
            }
        }
        default:
            break;
    }
}

void read_machine_data_callback(void *ctx, char *key, char *value) {
    Machine *machine = (Machine *) ctx;

    APP_LOG(APP_LOG_LEVEL_DEBUG, "workout key -> val: %s -> %s", key, value);

    switch (key[0]) {
        case 'm': {
            if (machine->mkey != (key[1] - 'A')) {
                APP_LOG(APP_LOG_LEVEL_DEBUG, "invalid machine index: %d", (key[1] - 'A'));
                break;
            }
            machine_data_load(machine, value);
        }
            break;
        default:
            break;
    }
}

void workout_load_current_without_machines(Workout *workout) {
    persist_read_string(DATA_WORKOUT_CURRENT, res, 200);

    read_key_values_unsafe(workout, res, read_workout_data_callback);
}

void workout_load_current(Workout *workout) {
    workout_load_current_without_machines(workout);

    Machine *m = workout->first_machine;

    for (int mi = 0; mi < M__COUNT; mi++) {
        if (m == NULL) break;

        persist_read_string(DATA_WORKOUT_CURRENT + 1 + mi, res, 200);
        read_key_values_unsafe(m, res, read_machine_data_callback);

        m = m->next;
    }
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
    if (w->first_machine != NULL) {
        machines_destroy(w->first_machine);
    }
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

Workout *workout_create_without_machines() {
    Workout *w = malloc(sizeof(Workout));
    w->time_start = 0;
    w->time_end = 0;
    w->location = -1;
    w->first_machine = NULL;
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
