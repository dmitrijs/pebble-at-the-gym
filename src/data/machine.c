#include <pebble.h>
#include "machine.h"
#include "../lib/key_value_unsafe.h"

static uint8_t buf[256];
static char res[201];
static char tmp[201];

static void _machines_destroy(Machine *first_machine);

void machine_serialize_for_upload(char *res, Machine *m) {
    snprintf(res, 200, "id=%d ww=%d wn=%d ", m->mkey, m->warmup_kg, m->normal_kg);
    snprintf(tmp, 200, "s1=%d s2=%d s3=%d di=%d dt=%ld;", m->set_1, m->set_2, m->set_3, (m->is_done ? 1 : 0), m->time_done);
    strcat(res, tmp);
}

void workout_serialize_for_upload(char *res, Workout *w) {
    snprintf(res, 200, "wl=%c ws=%ld we=%ld;", w->location, w->time_start, w->time_end);
}

static void dump(Workout *w, uint8_t *buf, int len) {
    workout_serialize_for_upload(res, w);
    APP_LOG(APP_LOG_LEVEL_WARNING, "workout: %s", res);

    Machine *m = w->first_machine;
    while (m != NULL) {
        machine_serialize_for_upload(res, m);
        APP_LOG(APP_LOG_LEVEL_WARNING, "machine: %s", res);
        m = m->next;
    }

    for (int i = 0; i < len; i++) {
        APP_LOG(APP_LOG_LEVEL_DEBUG, "buf[%d] = %d", i, buf[i]);
    }
}

static uint8_t _workout_serialize_version1(uint8_t *buf, Workout *w) {
    Serializer* ser = serializer_create(buf);

    serializer_write_uint8(ser, 1); // serialization version
    serializer_write_uint8(ser, (uint8_t) w->location);
    serializer_write_long(ser, w->time_start);
    serializer_write_long(ser, w->time_end);

    serializer_write_uint8(ser, M__COUNT);

    Machine *m = w->first_machine;
    while (m != NULL) {
        serializer_write_uint8(ser, (uint8_t) m->mkey);
        serializer_write_uint8(ser, m->warmup_kg);
        serializer_write_uint8(ser, m->normal_kg);
        serializer_write_uint8(ser, m->set_1);
        serializer_write_uint8(ser, m->set_2);
        serializer_write_uint8(ser, m->set_3);

        serializer_write_uint8(ser, (uint8_t) (m->is_done ? 1 : 0));
        serializer_write_long(ser, m->time_done);

        m = m->next;
    }

    serializer_destroy(ser);

    // dump(w, buf, 200);

    return ser->size;
}

static void _workout_save_to_key(Workout *w, bool deep, uint32_t data_key) {
    APP_LOG(APP_LOG_LEVEL_WARNING, "saving workout to slot: %d", (int) data_key);

    uint8_t size = _workout_serialize_version1(buf, w);

    persist_write_data(data_key, buf, size);
}

uint8_t workout_serialize_for_test(uint8_t *buf, Workout *w) {
    return _workout_serialize_version1(buf, w);
}

void workout_save_current(Workout *w, bool deep) {
    _workout_save_to_key(w, deep, DATA_WORKOUT_CURRENT);
}

static void _workout_unserialize_version1(uint8_t *buf, Workout *w) {
    Serializer* ser = serializer_create(buf);

    serializer_read_uint8(ser); // serialization version
    w->location = serializer_read_uint8(ser);
    w->time_start = serializer_read_long(ser);
    w->time_end = serializer_read_long(ser);

    uint8_t count = serializer_read_uint8(ser);

    if (M__COUNT != count) {
        APP_LOG(APP_LOG_LEVEL_ERROR, "Unserialized M__COUNT (%d) doesn't match current M__COUNT (%d)!", count, M__COUNT);
    }

    Machine *m = w->first_machine;
    while (m != NULL) {
        uint8_t mkey = serializer_read_uint8(ser);
        if (m->mkey != mkey) {
            APP_LOG(APP_LOG_LEVEL_ERROR, "Unserialized mkey (%d) doesn't match current mkey (%d)!", mkey, m->mkey);
        }
        m->warmup_kg = serializer_read_uint8(ser);
        m->normal_kg = serializer_read_uint8(ser);

        m->set_1 = serializer_read_uint8(ser);
        m->set_2 = serializer_read_uint8(ser);
        m->set_3 = serializer_read_uint8(ser);

        m->is_done = serializer_read_uint8(ser) == 1;
        m->time_done = serializer_read_long(ser);

        m = m->next;
    }

    serializer_destroy(ser);

    // dump(w, buf, 0);
}

static void _workout_load(Workout *workout, uint32_t data_position) {
    if (!persist_exists(data_position)) {
        APP_LOG(APP_LOG_LEVEL_ERROR, "Tried to load workout from position %lu which does not exist.", data_position);
        return;
    }

    uint8_t buf[256];
    persist_read_data(data_position, buf, 256);

    if (buf[0] == 1) {
        _workout_unserialize_version1(buf, workout);
    } else {
        APP_LOG(APP_LOG_LEVEL_ERROR, "Cannot unserialize data. Unknown version: %d", buf[0]);
    }
}

void workout_unserialize_for_test(uint8_t* buf, Workout *workout) {
    _workout_unserialize_version1(buf, workout);
}

void workout_load_current(Workout *workout) {
    _workout_load(workout, DATA_WORKOUT_CURRENT);
}

void workout_load_archived(Workout *workout, uint32_t data_position) {
    _workout_load(workout, data_position);
}

static void _machines_destroy(Machine *first_machine) {
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
        _machines_destroy(w->first_machine);
    }
    free(w);
}

static void _workout_reset(Workout *w, bool deep) {
    w->time_start = 0;
    w->time_end = 0;
    w->location = '?';

    if (deep) {
        Machine *m = w->first_machine;
        while (m != NULL) {
            m->is_done = false;
            m->time_done = 0;

            m = m->next;
        }
    }
}

void workout_cancel_current() {
    Workout *w = workout_create();
    workout_load_current(w);

    _workout_reset(w, /*deep*/true);
    workout_save_current(w, /*deep*/true);

    workout_destroy(w);
}

static Machine *_machines_create_all() {

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

Workout *workout_create() {
    Workout *w = malloc(sizeof(Workout));
    _workout_reset(w, /*deep*/false);
    w->first_machine = _machines_create_all();
    return w;
}

static void _state_read_callback(void *ctx, char *key, char *value) {
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
    read_key_values_unsafe(&state, res, _state_read_callback);

    return state;
}

static void _slots_save_state(SaveState state) {
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

    _workout_save_to_key(w, /*deep*/true, data_position);
    _slots_save_state(state);

    _workout_reset(w, /*deep*/true);
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

    APP_LOG(APP_LOG_LEVEL_INFO, "persist_delete %lu", (unsigned long)data_position);
    if (persist_exists(data_position)) {
        persist_delete(data_position);
    }

    _slots_save_state(state);
}
