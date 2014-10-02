#pragma once

#include <stdbool.h>

enum MACHINE_TYPES {
    M_WARMUP,
    M_SHOULDERS,
    M_CHEST,
    M_TRICEPS,
    M_BICEPS,
    M_UPPER_BACK,
    M_LOWER_BACK,
    M_LEGS_ULTIMATE,
//    M_LEGS_UP, /* excluded for now */
//    M_LEGS_DOWN,
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
    DATA_WORKOUT = 0
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
    uint16_t time_done;

    Machine *next;
    Machine *prev;
};

typedef struct Workout Workout;
struct Workout {
    uint16_t time_start;
    uint16_t time_end;

    int location;

    bool m_done[M__COUNT];
    Machine *first_machine;
};

Workout workout_load_current();

void workout_save(Workout);

void machines_data_load(Machine *first_machine);

void machines_data_save(Machine *first_machine);

void machines_destroy(Machine *first_machine);

Machine *machines_create_all();
