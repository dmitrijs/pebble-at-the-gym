#include <pebble.h>
#include "machine.h"
#include "../lib/key_value_unsafe.h"

char res[201];
char tmp[201];

Machine *machines_create_all();

void machines_destroy(Machine *first_machine);

void machine_serialize(char *res, Machine *m) {
    snprintf(res, 200, "id=%d ww=%d wn=%d ", m->mkey, m->warmup_kg, m->normal_kg);
    snprintf(tmp, 200, "s1=%d s2=%d s3=%d di=%d dt=%ld;", m->set_1, m->set_2, m->set_3, (m->is_done ? 1 : 0), m->time_done);
    strcat(res, tmp);
}

void workout_serialize(char *res, Workout *w) {
    snprintf(res, 200, "wl=%c ws=%ld we=%ld;", w->location, w->time_start, w->time_end);
}

void machine_save_to_key(Machine *m, uint32_t data_key) {
    machine_serialize(res, m);

    persist_write_string((data_key + 1 + m->mkey), res);
    APP_LOG(APP_LOG_LEVEL_WARNING, "machine: %s", res);
}

void machine_save_current(Machine *m) {
    machine_save_to_key(m, DATA_WORKOUT_CURRENT);
}

void workout_save_to_key(Workout *w, bool deep, uint32_t data_key) {
    APP_LOG(APP_LOG_LEVEL_WARNING, "saving workout to slot: %d, deep: %d", (int) data_key, (deep ? 1 : 0));

    workout_serialize(res, w);
    APP_LOG(APP_LOG_LEVEL_WARNING, "workout: %s", res);
    persist_write_string(data_key, res);

    if (deep) {
        Machine *m = w->first_machine;
        while (m != NULL) {
            machine_save_to_key(m, data_key);
            m = m->next;
        }
    }
}

void workout_save_current(Workout *w, bool deep) {
    workout_save_to_key(w, deep, DATA_WORKOUT_CURRENT);
}

void read_workout_data_callback(void *ctx, char *key, char *value) {
    Workout *workout = (Workout *) ctx;

//    APP_LOG(APP_LOG_LEVEL_DEBUG, "workout key -> val: %s -> %s", key, value);

    switch (key[0]) {
        case 'w': {
            switch (key[1]) {
                case 'l':
                    workout->location = value[0];
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

//    APP_LOG(APP_LOG_LEVEL_DEBUG, "machine key -> val: %s -> %s", key, value);

    switch (key[0]) {
        case 'i': // id
            if (key[1] == 'd') {
                if (machine->mkey != atoi(value)) {
                    APP_LOG(APP_LOG_LEVEL_DEBUG, "LOADED INVALID MACHINE INDEX: %d", (key[1] - 'A'));
                }
            }
            break;

        case 'w': // weights
            switch (key[1]) {
                case 'w':
                    machine->warmup_kg = atoi(value);
                    break;
                case 'n':
                    machine->normal_kg = atoi(value);
                    break;
                default:
                    break;
            }
            break;

        case 's': // sets
            switch (key[1]) {
                case '1':
                    machine->set_1 = atoi(value);
                    break;
                case '2':
                    machine->set_2 = atoi(value);
                    break;
                case '3':
                    machine->set_3 = atoi(value);
                    break;
                default:
                    break;
            }
            break;

        case 'd': // done
            switch (key[1]) {
                case 'i':
                    machine->is_done = atoi(value) == 1;
                    break;
                case 't':
                    machine->time_done = atol(value);
                    break;
                default:
                    break;
            }
            break;

        default:
            break;
    }
}

void workout_load_by_data_position_without_machines(Workout *workout, uint32_t data_position) {
    persist_read_string(data_position, res, 200);

    read_key_values_unsafe(workout, res, read_workout_data_callback);
}

void workout_load_current_without_machines(Workout *workout) {
    workout_load_by_data_position_without_machines(workout, DATA_WORKOUT_CURRENT);
}

void workout_load_by_data_position(Workout *workout, uint32_t data_position) {
    workout_load_by_data_position_without_machines(workout, data_position);

    Machine *machine = workout->first_machine;

    for (int mi = 0; mi < M__COUNT; mi++) {
        if (machine == NULL) break;

        persist_read_string(data_position + 1 + mi, res, 200);
        read_key_values_unsafe(machine, res, read_machine_data_callback);

        machine = machine->next;
    }
}

void workout_load_current(Workout *workout) {
    workout_load_by_data_position(workout, DATA_WORKOUT_CURRENT);
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

void machine_reset(Machine *m) {
    m->is_done = false;
    m->time_done = 0;
}

void workout_reset(Workout *w, bool deep) {
    w->time_start = 0;
    w->time_end = 0;
    w->location = '?';

    if (deep) {
        Machine *m = w->first_machine;
        while (m != NULL) {
            machine_reset(m);
            m = m->next;
        }
    }
}

void workout_cancel_current() {
    Workout *w = workout_create();
    workout_load_current(w);

    workout_reset(w, /*deep*/true);
    workout_save_current(w, /*deep*/true);

    workout_destroy(w);
}

Workout *workout_create() {
    Workout *w = workout_create_without_machines();
    w->first_machine = machines_create_all();
    return w;
}

Workout *workout_create_without_machines() {
    Workout *w = malloc(sizeof(Workout));
    workout_reset(w, /*deep*/false);
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

void state_read_callback(void *ctx, char *key, char *value) {
    SaveState *state = (SaveState *) ctx;

    switch (key[0]) {
        case '1':
            state->save1_in_use = atoi(value) == 1;
            break;
        case '2':
            state->save2_in_use = atoi(value) == 1;
            break;
        case '3':
            state->save3_in_use = atoi(value) == 1;
            break;
        default:
            break;
    }
}

SaveState slots_load_state() {
    persist_read_string(DATA_WORKOUT_SAVE_STATE, res, 200);

    SaveState state = (SaveState) {
            .save1_in_use = false,
            .save2_in_use = false,
            .save3_in_use = false
    };
    read_key_values_unsafe(&state, res, state_read_callback);

    return state;
}

void slots_save_state(SaveState state) {
    snprintf(res, 200, "1=%d;2=%d;3=%d;", state.save1_in_use ? 1 : 0, state.save2_in_use ? 1 : 0, state.save3_in_use ? 1 : 0);

    APP_LOG(APP_LOG_LEVEL_WARNING, "updating state: %s", res);
    persist_write_string(DATA_WORKOUT_SAVE_STATE, res);
}

bool workout_end_current() {
    // return false and do no changes if no slot was available to backup data
    SaveState state = slots_load_state();

    uint32_t data_position;
    if (!state.save1_in_use) {
        data_position = DATA_WORKOUT_SAVE_1;
        state.save1_in_use = true;
    } else if (!state.save2_in_use) {
        data_position = DATA_WORKOUT_SAVE_2;
        state.save2_in_use = true;
    } else if (!state.save3_in_use) {
        data_position = DATA_WORKOUT_SAVE_3;
        state.save3_in_use = true;
    } else {
        return false;
    }

    Workout *w = workout_create();
    workout_load_current(w);

    workout_save_to_key(w, /*deep*/true, data_position);
    slots_save_state(state);

    workout_reset(w, /*deep*/true);
    workout_save_current(w, /*deep*/true);

    workout_destroy(w);

    return true;
}

void workout_delete_by_slot(uint16_t slot_number) {
    SaveState state = slots_load_state();

    uint32_t data_position;
    switch (slot_number) {
        case 1:
            data_position = DATA_WORKOUT_SAVE_1;
            state.save1_in_use = false;
            break;
        case 2:
            data_position = DATA_WORKOUT_SAVE_2;
            state.save2_in_use = false;
            break;
        case 3:
            data_position = DATA_WORKOUT_SAVE_3;
            state.save3_in_use = false;
            break;
        default:
            return;
    }

    persist_delete(data_position);

    for (int i = 0; i < M__COUNT; i++) {
        persist_delete(data_position + 1 + i);
    }

    slots_save_state(state);
}
