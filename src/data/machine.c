#include <pebble.h>
#include "machine.h"
#include "../lib/parsed.h"
#include "../lib/persistor.h"

char data_buffer[300];

Machine *machines_create_all();

void machines_destroy(Machine *first_machine);

void machines_data_save(Machine *first_machine);

void machines_data_load(Machine *first_machine);

void workout_save_current(Workout *w) {
    machines_data_save(w->first_machine);
    // persist_write_long_string(DATA_WORKOUT_CURRENT, data_buffer);
}

bool workout_try_backup(Workout *w) {
    return false;
}

void workout_load_current(Workout *workout) {
    machines_data_load(workout->first_machine);
    return;
    /*persist_write_string(DATA_WORKOUT_CURRENT, "15;10000;12000;X-X--XXX-X-;");
    persist_read_string(DATA_WORKOUT_CURRENT, data_buffer, 256);

    Workout w = (Workout) {
            .location = 0
    };

    parsed *p = parsed_create(data_buffer, ';');
    if (parsed_done(p)) {
//        return w;
    }

    w.location = parsed_number(p);
    w.time_start = parsed_number(p);
    w.time_end = parsed_number(p);

    char *tmp = malloc(M__COUNT + 1);
    parsed_string(p, tmp, M__COUNT);

    for (int i = 0; i < M__COUNT; i++) {
//        w.m_done[i] = tmp[i] == 'X';
    }

    free(tmp);
    free(p);

    machines_data_load(workout->first_machine);

//    return w;*/
}

void machines_data_load(Machine *first_machine) {
    Machine *m = first_machine;

    persist_read_string(DATA_WORKOUT_CURRENT, data_buffer, 256);
    parsed *p = parsed_create(data_buffer, ';');

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
        parsed_skip(p, 1);

        m = m->next;
    }
    free(p);
}

void machines_data_save(Machine *first_machine) {
    // "0;100;101;10;11;12;;1;102;103;13;14;15;;";
    data_buffer[0] = 0;

    char tmp[24];

    Machine *m = first_machine;
    while (m != NULL) {
        snprintf(tmp, 24, "%d;%d;%d;%d;%d;%d;;", m->mkey, m->warmup_kg, m->normal_kg, m->set_1, m->set_2, m->set_3);
        strcat(data_buffer, tmp);
        m = m->next;
    }
    if (strlen(data_buffer) > 255) {
        APP_LOG(APP_LOG_LEVEL_ERROR, "data is too big. needs truncation.");
        return;
    }

    persist_write_string(DATA_WORKOUT_CURRENT, data_buffer);
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
