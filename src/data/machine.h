#pragma once

#include <pebble.h>

enum MACHINE_TYPES {
    M_WARMUP,
    M_SHOULDERS,
    M_CHEST,
    M_TRICEPS,
    M_BICEPS,
    M_UPPER_BACK,
    M_LOWER_BACK,
    M_LEGS_ULTIMATE,
    M_ABS,
    M_ABS_SIDE,
    M_COOLDOWN,
    M__COUNT
};
enum FIELD_TYPE {
    F_TITLE,
    F_WARMUP_KG,
    F_NORMAL_KG,
    F_SET_1,
    F_SET_2,
    F_SET_3,
    F__COUNT
};

enum {
    DATA_WORKOUT_CURRENT = 0, // workout + 12 machines = 13 indices
    DATA_WORKOUT_SAVE_STATE = 100,
    DATA_WORKOUT_SAVE_1 = 200,
    DATA_WORKOUT_SAVE_2 = 300,
    DATA_WORKOUT_SAVE_3 = 400
};

typedef struct Machine Machine;
struct Machine {
    int mkey;
    char *title;
    int warmup_kg;
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
    long time_done;

    Machine *next;
    Machine *prev;
};
// data size: 1char (key) 1 byte (warmup kg) 1 byte (normal kg) 3x1 byte (3 sets) 1 byte (done) 4bytes(time) = 11 bytes

typedef struct SaveState SaveState;
struct SaveState {
    bool save1_in_use;
    bool save2_in_use;
    bool save3_in_use;
};
// TODO: replace with "persist_exists"

typedef struct Workout Workout;
struct Workout {
    long time_start;
    long time_end;

    char location;

    Machine *first_machine;
};
// data size: 1char(location)+4bytes(start)+4bytes(end)=11 bytes

void machine_serialize(char *res, Machine *m);
void workout_serialize(char *res, Workout *w);
void workout_load_by_data_position(Workout *workout, uint32_t data_position);
void workout_load_current_without_machines(Workout *workout);
void workout_save_current(Workout *w, bool deep);
void machine_save_current(Machine *m);
void workout_serialize(char *res, Workout *w);
void machine_serialize(char *res, Machine *m);
Workout *workout_create_without_machines();
Workout *workout_create();
void workout_cancel_current();
void workout_destroy(Workout *w);
void workout_load_current(Workout *workout);
void workout_delete_by_slot(uint16_t slot_number);
bool workout_end_current();
SaveState slots_load_state();
